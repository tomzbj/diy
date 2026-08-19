#include "misc.h"
#include "platform.h"
#include "st7789.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "ui.h"

/* static void adjust_brightness(void* pdata)
 {   const int brightness[] = {49, 67, 90, 122, 165, 222, 300, 406, 548, 740, 999};
 int val = saturate(*(int*)pdata, 0, 10);
 TIMER_SetLCDPWM(brightness[val]);
 TIMER_SetLCDPWM(950);
 ZPin::set (PA8); }*/

auto setcs = [](int s) {ZPin::write(PA4, s);};
//auto setcs = [](int s) {};    //ZPin::write(PB7, s);};
auto setrs = [](int r) {ZPin::write(PA3, r);};    // for ser mode
auto reset = [](int n) {ZPin::write(PA6, n);};

static TFT::cbs_t cbs = {reset, setcs, setrs, (void*)SPI1_Write, _delay_us};
//static TFT::cbs_t cbs = {reset, setcs, setrs, (void*)SPI1_Write_DMA, _delay_us};
TFT_ST7789 tft(240, 240, TFT::DATAWIDTH_8, cbs);

void DISP_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);

//  ZPin::clear (PB7);    // clear cs
  tft.init();
  tft.clear(0xffff);

  lv_init();
  lv_port_disp_init();
  print_log();
}
