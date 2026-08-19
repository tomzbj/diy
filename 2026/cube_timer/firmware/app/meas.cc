#include "meas.h"
#include "zi2c.h"
#include "mma7660.h"
#include "st7789.h"
#include "lvgl.h"
#include "platform.h"

extern class TFT_ST7789 tft;

// SDA=PB5 (OD + pull-up), SCL=PB6 (PP: no board pull-up)
// MMA7660 INT → PA0/WKUP (push-pull + active-high, no board R)
ZI2C i2c(PB5, PB6, 0x4c, _delay_us, 60);
MMA7660 mma(i2c);

static bool g_mma_ok = false;
static MMA7660::ORIENT g_orient = MMA7660::ORIENT::UNKNOWN;
static MMA7660::ORIENT g_face = MMA7660::ORIENT::UNKNOWN;    // committed face
static int g_face_stable = 0;
static int g_front_stable = 0;
static int g_sleep_inhibit = 0;

static uint32_t g_deadline_ms = 0;    // millis() when remain hits 0; 0 = idle/alarm
static int g_last_shown_sec = -1;
static volatile bool g_alarming = false;
static volatile uint32_t g_alarm_ms = 0;    // advanced in SysTick

static constexpr int ORIENT_CONE_DEG = 25;
static constexpr int FACE_COMMIT_TICKS = 4;     // ~1s @ 250ms before apply preset
static constexpr int FRONT_SLEEP_TICKS = 8;     // ~2s on F before STANDBY
static constexpr int SLEEP_INHIBIT_TICKS = 12;  // ~3s after WKUP

static constexpr int BACK_DURATION_SEC = 5 * 60;

// U/D/L/R/B → seconds (F = sleep, not a timer)
static int orient_duration_sec(MMA7660::ORIENT o)
{
  switch(o) {
    case MMA7660::ORIENT::BACK:
      return BACK_DURATION_SEC;
    case MMA7660::ORIENT::UP:
      return 10 * 60;
    case MMA7660::ORIENT::DOWN:
      return 15 * 60;
    case MMA7660::ORIENT::LEFT:
      return 20 * 60;
    case MMA7660::ORIENT::RIGHT:
      return 30 * 60;
    default:
      return 0;
  }
}

static char orient_char(MMA7660::ORIENT o)
{
  switch(o) {
    case MMA7660::ORIENT::UP:
      return 'U';
    case MMA7660::ORIENT::DOWN:
      return 'D';
    case MMA7660::ORIENT::LEFT:
      return 'L';
    case MMA7660::ORIENT::RIGHT:
      return 'R';
    case MMA7660::ORIENT::FRONT:
      return 'F';
    case MMA7660::ORIENT::BACK:
      return 'B';
    default:
      return '#';
  }
}

static bool orient_is_timer(MMA7660::ORIENT o)
{
  return orient_duration_sec(o) > 0;
}

static void alarm_stop(void)
{
  g_alarming = false;
  g_alarm_ms = 0;
  TIMER5_SetDuty(0);
  UI_SetTextVisible(1);
}

static void alarm_start(void)
{
  g_alarm_ms = 0;
  g_alarming = true;
  UI_SetRemainSec(0);
  g_last_shown_sec = 0;
  printf("timer: alarm\n");
}

// MADCTL dirs for set_dir(0..3); tuned on hardware (R ok; U +180°; D/L +90° CCW)
static int orient_to_dir(MMA7660::ORIENT o)
{
  switch(o) {
    case MMA7660::ORIENT::UP:
      return 2;
    case MMA7660::ORIENT::DOWN:
      return 0;
    case MMA7660::ORIENT::LEFT:
      return 1;
    case MMA7660::ORIENT::RIGHT:
      return 3;
    default:
      return -1;
  }
}

static void delay_ms(int ms)
{
  while(ms--)
    _delay_us(1000);
}

static void i2c_pins_idle(void)
{
  ZPin::set(PB5);
  ZPin::set(PB6);
}

static void i2c_bus_recover(void)
{
  i2c_pins_idle();
  for(int i = 0; i < 9; i++) {
    ZPin::set(PB6);
    _delay_us(60);
    ZPin::clear(PB6);
    _delay_us(60);
  }
  ZPin::clear(PB5);
  _delay_us(60);
  ZPin::set(PB6);
  _delay_us(60);
  ZPin::set(PB5);
  _delay_us(60);
}

static void mma_gpio_init(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_6;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);

  gis.Pin = GPIO_PIN_5;
  gis.Mode = GPIO_MODE_OUTPUT_OD;
  gis.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &gis);

  i2c_pins_idle();
}

static bool mma_reinit(void)
{
  mma_gpio_init();
  i2c_bus_recover();
  delay_ms(5);

  for(int i = 0; i < 4; i++) {
    mma.clear_int();
    delay_ms(2);
  }

  mma.set_mode(MMA7660::MODE::STANDBY);
  delay_ms(2);
  mma.set_intsu(0);
  mma.set_sample_rate(0, 0);
  delay_ms(2);

  for(int i = 0; i < 5; i++) {
    i2c_bus_recover();
    mma.set_mode(MMA7660::MODE::STANDBY);
    delay_ms(5);
    uint8_t mode = mma.read_mode();
    if(mode == 0x00) {
      mma.set_mode(MMA7660::MODE::ACTIVE);
      mma.clear_int();
      return true;
    }
    printf("mma: mode=%02x int=%d retry %d\n", mode, PWR_ReadWkupPin(), i);
    delay_ms(10);
  }
  return false;
}

static bool wait_int_quiet(int settle_ms, int max_rearm)
{
  for(int attempt = 0; attempt <= max_rearm; attempt++) {
    mma.clear_int();
    int left = settle_ms;
    bool quiet = true;
    while(left > 0) {
      delay_ms(10);
      left -= 10;
      if(PWR_ReadWkupPin()) {
        quiet = false;
        break;
      }
    }
    if(quiet)
      return true;
    printf("mma: INT glitch during settle, retry %d\n", attempt);
  }
  return false;
}

// From F: wake only on UDLR (PLINT).
// Do NOT arm FBINT — Front re-detect / BaFro chatter wakes immediately after STANDBY.
// Tip to B usually passes through a side face and still trips PLINT.
static bool mma_arm_wakeup(void)
{
  mma.set_mode(MMA7660::MODE::STANDBY);
  mma.set_intsu(MMA7660::INT_PLINT);
  mma.set_sample_rate(3, 7);    // 16Hz + FILT=7
  mma.set_mode(MMA7660::MODE::ACTIVE, MMA7660::MODE_IAH | MMA7660::MODE_IPP);

  delay_ms(150);

  // Software already decided F (Z~+1g). Chip BaFro may stay Unknown (tilt=00) on
  // this board — only require PoLa clear so PLINT will not fire while flat.
  for(int i = 0; i < 10; i++) {
    uint8_t tilt = 0;
    if(mma.read_tilt(tilt) && ((tilt >> 2) & 0x7) == 0)
      break;
    printf("mma: wait PoLa clear tilt=%02x\n", tilt);
    delay_ms(50);
    if(i == 9) {
      printf("mma: PoLa stuck, abort standby\n");
      (void)mma_reinit();
      return false;
    }
  }

  if(!wait_int_quiet(500, 5)) {
    printf("mma: INT unstable, abort standby\n");
    (void)mma_reinit();
    return false;
  }

  if(PWR_ReadWkupPin()) {
    printf("mma: INT still high, abort standby\n");
    (void)mma_reinit();
    return false;
  }

  i2c_pins_idle();
  return true;
}

static void try_enter_standby(void)
{
  printf("standby: arm PLINT only...\n");
  if(!mma_arm_wakeup()) {
    g_front_stable = 0;
    return;
  }
  // Last check: pin must be low or WKUP fires on entry
  if(PWR_ReadWkupPin()) {
    printf("standby: abort, INT high at entry\n");
    (void)mma_reinit();
    g_front_stable = 0;
    return;
  }
  printf("standby: sleeping (tip U/D/L/R to wake)\n");
  alarm_stop();
  ADC_PrepareStandby();    // PA11 high — stop divider bleed
  TIM3->CCR3 = 0;
  delay_ms(10);
  if(PWR_ReadWkupPin()) {
    printf("standby: abort, INT rose before WFI\n");
    (void)mma_reinit();
    g_front_stable = 0;
    return;
  }
  PWR_EnterStandby();
}

static void timer_start_sec(int sec)
{
  alarm_stop();
  g_deadline_ms = millis() + (uint32_t)sec * 1000UL;
  g_last_shown_sec = -1;
  printf("timer: %d sec\n", sec);
}

static void timer_update_ui(void)
{
  int sec = 0;
  if(g_deadline_ms != 0) {
    uint32_t now = millis();
    if(now < g_deadline_ms)
      sec = (int)((g_deadline_ms - now + 999) / 1000);    // ceil
    else {
      sec = 0;
      g_deadline_ms = 0;
      if(!g_alarming)
        alarm_start();
    }
  }
  else if(g_alarming)
    sec = 0;

  if(!g_alarming && sec != g_last_shown_sec) {
    g_last_shown_sec = sec;
    UI_SetRemainSec(sec);
  }
}

static void apply_face(MMA7660::ORIENT o)
{
  g_face = o;
  int sec = orient_duration_sec(o);
  if(sec > 0)
    timer_start_sec(sec);

  int dir = orient_to_dir(o);
  if(dir >= 0) {
    tft.set_dir(dir);
    tft.clear(0x0000);    // wipe old pixels; MADCTL rotate does not clear GRAM
    if(lv_screen_active())
      lv_obj_invalidate(lv_screen_active());
    printf("disp dir=%d\n", dir);
  }
}

// SysTick @ 1ms: even Morse H (....) — 50ms on / 50ms off ×4, then quiet to 1s
void ALARM_Systick(void)
{
  if(!g_alarming)
    return;

  uint32_t t = g_alarm_ms % 1000UL;
  bool beep = false;
  for(int i = 0; i < 4; i++) {
    uint32_t start = (uint32_t)i * 100UL;
    if(t >= start && t < start + 50UL)
      beep = true;
  }
  // Direct CCR write — keep ISR free of LVGL / printf
  TIM5->CCR2 = beep ? ((TIM5->ARR + 1U) / 2U) : 0U;
  g_alarm_ms++;
}

int MEAS_IsAlarming(void)
{
  return g_alarming ? 1 : 0;
}

// Main loop: blink + LVGL while alarming (normal UI_Task is slowed)
void ALARM_Task(void)
{
  if(!g_alarming)
    return;
  UI_SetTextVisible(((g_alarm_ms / 250UL) % 2UL) == 0UL ? 1 : 0);
  lv_tick_inc(50);
  lv_timer_handler();
}

void MEAS_Poll(void)
{
  if(!g_mma_ok)
    return;

  if(g_sleep_inhibit > 0)
    g_sleep_inhibit--;

  MMA7660::ACC acc;
  if(!mma.read_acc(acc)) {
    printf("mma: acc alert timeout\n");
    return;
  }

  MMA7660::ORIENT prev = g_orient;
  g_orient = mma.orient_update(acc, g_orient, ORIENT_CONE_DEG);

  if(g_orient != prev) {
    g_face_stable = 0;
    g_front_stable = 0;
  }

  printf("%d %d %d %c", acc.x, acc.y, acc.z, orient_char(g_orient));
  if(g_orient != prev)
    printf(" <-%c", orient_char(prev));

  if(g_orient == MMA7660::ORIENT::FRONT) {
    g_face_stable = 0;
    if(g_sleep_inhibit > 0) {
      printf(" inhibit=%d\n", g_sleep_inhibit);
    }
    else {
      if(g_front_stable < FRONT_SLEEP_TICKS)
        g_front_stable++;
      printf(" F=%d/%d\n", g_front_stable, FRONT_SLEEP_TICKS);
      if(g_front_stable >= FRONT_SLEEP_TICKS)
        try_enter_standby();
    }
  }
  else if(orient_is_timer(g_orient)) {
    g_front_stable = 0;
    if(g_face_stable < FACE_COMMIT_TICKS)
      g_face_stable++;
    printf(" face=%d/%d", g_face_stable, FACE_COMMIT_TICKS);
    if(g_face_stable >= FACE_COMMIT_TICKS && g_orient != g_face) {
      apply_face(g_orient);
      printf(" ->%c %ds", orient_char(g_orient), orient_duration_sec(g_orient));
    }
    printf("\n");
  }
  else {
    g_front_stable = 0;
    g_face_stable = 0;
    printf("\n");
  }

  timer_update_ui();
}

void MEAS_Config(void)
{
  bool from_standby = PWR_FlagStandby();
  bool from_wkup = PWR_FlagWkup();
  if(from_standby)
    printf("wake: from STANDBY\n");
  if(from_wkup)
    printf("wake: WKUP pin\n");
  PWR_Config();

  if(from_standby || from_wkup) {
    delay_ms(20);
    g_sleep_inhibit = SLEEP_INHIBIT_TICKS;
  }

  if(!mma_reinit()) {
    g_mma_ok = false;
    printf("mma: probe fail (bus/chip?)\n");
    print_log();
    return;
  }

  g_mma_ok = true;
  g_orient = MMA7660::ORIENT::UNKNOWN;
  g_face = MMA7660::ORIENT::UNKNOWN;
  g_face_stable = 0;
  g_front_stable = 0;
  g_deadline_ms = 0;
  g_last_shown_sec = -1;
  alarm_stop();
  if(!(from_standby || from_wkup))
    g_sleep_inhibit = 0;
  UI_SetRemainSec(0);
  UI_SetTextVisible(1);
  ADC_DividerEnable(1);    // after wake from STANDBY
  printf("mma: ok F=STANDBY B=%ds U/D/L/R=10/15/20/30min\n",
      BACK_DURATION_SEC);
  print_log();
}
