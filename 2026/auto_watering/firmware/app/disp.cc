#include "misc.h"
#include "platform.h"
#include "gc9307.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "ui.h"

auto setcs = [](int s) {ZPin::write(PB7, s);};
auto setrs = [](int r) {ZPin::write(PB6, r);};    // for ser mode
auto reset = [](int n) {ZPin::write(PB8, n);};

//static TFT::cbs_t cbs = {reset, setcs, setrs, (void*)SPI3_Write, _delay_us};
static TFT::cbs_t cbs = {reset, setcs, setrs, (void*)SPI3_Write_DMA, _delay_us};
TFT_GC9307 tft(320, 172, TFT::DATAWIDTH_8, cbs);
UI ui;

struct EEPROM {
    uint32_t time_next, duration, brightness;
} eeprom;

static void adjust_brightness_f(void* pdata)
{
  const int brightness[] = {49, 67, 90, 122, 165, 222, 300, 406, 548, 740, 999};
  int val = saturate(*(int*)pdata, 0, 10);
  TIM2->CCR1 = brightness[val];    //  ZPin::set (PA8);
}

static void get_vals_f(void* pvals)
{
  auto vals = (UI::VALS*)pvals;
  extern uint32_t now;
  vals->time = now;
  vals->time_next = ui.cfgs.time_next;
  vals->duration = ui.cfgs.duration;
  vals->moisture = MOISTURE_Get();
}

static void save_cfg_f(void* pcfgs)
{
  auto p = (UI::CFGS*)pcfgs;
  eeprom.brightness = p->brightness;
  eeprom.duration = p->duration;
  eeprom.time_next = p->time_next;
  now = p->time;
  EEPROM_Write(&eeprom, sizeof(eeprom));
  if(0) {
    FILE* f = fopen("eeprom.bin", "wb");
    if(f == nullptr)
      return;
    fwrite(&eeprom, sizeof(eeprom), 1, f);
    fclose(f);
  }
}

static void load_cfg_f(void* pcfgs)
{
  if(0) {
    FILE* f = fopen("eeprom.bin", "rb");
    if(f == nullptr)
      return;
    fread(&eeprom, sizeof(eeprom), 1, f);
    fclose(f);
  }
  EEPROM_Read(&eeprom, sizeof(eeprom));
  auto p = (UI::CFGS*)pcfgs;
  p->time = now;
  p->brightness = eeprom.brightness;
  p->duration = eeprom.duration;
  p->time_next = eeprom.time_next;
}

static void manual_run_f(void* params)
{
  extern uint32_t motor_duration;
  motor_duration = 3;
  MOTOR_Set(MOTOR_RUN);
}

void DISP_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);

//  ZPin::clear (PB7);    // clear cs
  tft.init();
  tft.clear(0x0);

  lv_init();
  lv_port_disp_init();
  TIM2->CCR1 = 150;
  ui.init();
  ui.bind_cb(UI::CB_ID::GET_VALS, get_vals_f);
  ui.bind_cb(UI::CB_ID::LOAD_CFG, load_cfg_f);
  ui.bind_cb(UI::CB_ID::SAVE_CFG, save_cfg_f);
  ui.bind_cb(UI::CB_ID::ADJUST_BRIGHTNESS, adjust_brightness_f);
  ui.bind_cb(UI::CB_ID::MANUAL_RUN, manual_run_f);
  print_log();
}
