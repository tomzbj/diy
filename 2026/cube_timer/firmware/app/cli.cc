#include "misc.h"
#include "platform.h"
#include "st7789.h"
#include "lvgl.h"
#include "malloc.h"

#define ok() printf("ok\n");

static void cli_wr_clear(const char* tokens[])
{
  uint16_t c = strtoul(tokens[2], nullptr, 16);
  extern class TFT_ST7789 tft;
  auto t1 = DWT->CYCCNT;
  tft.clear(c);
  auto t2 = DWT->CYCCNT;
  printf("%lu\n", t2 - t1);
  ok();
}

static void cli_wr_pwm(const char* tokens[])
{
  _dbg();
  int pwm = atoi(tokens[2]);
  pwm = saturate(pwm, 0, 999);
  TIM3->CCR3 = pwm;
  ok();
}

static void cli_wr_rotate(void)
{
//  lv_disp_rotation_t dir[] = {LV_DISPLAY_ROTATION_0, LV_DISPLAY_ROTATION_90, LV_DISPLAY_ROTATION_180, LV_DISPLAY_ROTATION_270};
  static int count = 0;
  ++count %= 4;
  /*  lv_disp_t* disp = lv_disp_get_default();
   lv_disp_set_rotation(disp, dir[count]);
   lv_obj_invalidate (lv_screen_active());
   ok () ;*/
  extern class TFT_ST7789 tft;
  printf("## %d\n", count);
  tft.set_dir(count);
  ok();
}

static void cli_rd_sbrk_sp(const char* tokens[])
{
  register uint8_t* sp asm("sp");
  auto a2 = sp;
  auto a1 = malloc(1);
  printf("%08lx %08lx\n", (uint32_t)a1, (uint32_t)a2);
  if(a2 != nullptr)
    free(a1);

  struct mallinfo mi = mallinfo();
  printf("Used: %d, Free: %d\n", mi.uordblks, mi.fordblks);
  {
    struct _reent* r = _impure_ptr;
    printf("Reent pointer: %08lx\n", (uint32_t)(void*)r);
    printf("Reent size: %d\n", sizeof(struct _reent));

    // 如果 _stdout 不为空，说明标准 I/O 已经初始化并占用了内存
    if(r->_stdout != NULL) {
      printf("stdout is initialized at %08lx\n", (uint32_t)((void*)r->_stdout));
    }
  }
}

static void cli_wr_beep(const char* tokens[])
{
  int pwm = atoi(tokens[2]);
  TIMER5_SetDuty(pwm);
  ok();
}

static void cli_wr_reset(const char* tokens[])
{
  while(1) {
    ZPin::toggle(PA6);
    HAL_Delay(100);
    printf("#\n");
  }
}

void CLI_Config(void)
{
  cli.bind("test", []() {puts("TEST OK");});
  cli.bind("reboot", []() {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd", []() {printf("%lu\n", HAL_RCC_GetSysClockFreq());});

  cli.bind("clear", "wr", cli_wr_clear);
  cli.bind("pwm", "wr", cli_wr_pwm);

  cli.bind("rotate", "wr", cli_wr_rotate);
  cli.bind("sbrk_sp", "rd", cli_rd_sbrk_sp);

  cli.bind("beep", "wr", cli_wr_beep);
  cli.bind("reset", "wr", cli_wr_reset);

  print_log();
}
