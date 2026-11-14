#include "misc.h"
#include "platform.h"
#include "ssd1306_i2c.h"
#include "disp.h"
#include "ugui.h"
#include <cmath>

//auto nop = +[](volatile int n) {};
ZI2C i2c(PA11, PB2, 0x3c, _delay_us, 0);
SSD1306 oled(i2c, _delay_us, 128, 32);
uint8_t vram[512] = {0};
UG_GUI gui;

static void pset(int16_t x, int16_t y, uint16_t c)
{
  // Must reject x/y outside 128x64: ofs==sizeof(vram) is already OOB, and
  // map places .bss.charger immediately after vram.
//  if(x < 0 || x >= 128 || y < 0 || y >= 64) return;
  uint16_t y1 = (uint16_t)y >> 3;
  uint8_t yt = y & 0x7;
  uint32_t ofs = y1 * 128u + (uint16_t)x;
  if(ofs >= sizeof(vram))
    return;
  if(c)
    vram[ofs] |= (1 << yt);
  else
    vram[ofs] &= ~(1 << yt);
}

void DISP_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_11;
  gis.Mode = GPIO_MODE_OUTPUT_OD;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOB, &gis);

  ZPin::clear (PA6);
  HAL_Delay(200);
  ZPin::set(PA6);
  HAL_Delay(200);

  oled.init();
  oled.clear(0xff);

  UG_Init(&gui, pset, 128, 32);
  UG_SelectGUI(&gui);
  UG_FontSetHSpace(0);
  UG_FontSetVSpace(0);

  DISP_Update();

  print_log();
}

void DISP_Update(void)
{
  UG_FillScreen(0x00);
  char buf[16] = {0};

  UG_FontSelect(&FONT_8X16);
  float val[4] = {0};
  memcpy(val, ADC_Get(), sizeof(val));
  struct {
      int pos, x, y;
      char* unit;
  } item[4] = {    //
    {2, 0, 0, (char*)"V"}, {0, 64, 0, (char*)"mA"},    //
      {3, 0, 16, (char*)"V"}, {1, 64, 16, (char*)"mA"}};

  for(int i = 0; i < 4; i++) {
    sprintf(buf, "%f", val[item[i].pos]);
    buf[4] = '\0';
    if(buf[3] == '.')
      buf[3] = '\0';
    strcat(buf, item[i].unit);
    UG_PutString(item[i].x, item[i].y, buf);
  }

  oled.write(vram, sizeof(vram));
}
