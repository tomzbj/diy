#include "misc.h"
#include "platform.h"
#include "ssd1306_i2c.h"
#include "disp.h"
#include "ugui.h"
#include <cmath>

ZI2C i2c(PB7, PB6, 0x3c, _delay_us, 1);
SSD1306 oled(i2c, _delay_us);
uint8_t vram[1024] = {0};
UG_GUI gui;

static void pset(int16_t x, int16_t y, uint16_t c)
{
  // Must reject x/y outside 128x64: ofs==sizeof(vram) is already OOB, and
  // map places .bss.charger immediately after vram.
  if(x < 0 || x >= 128 || y < 0 || y >= 64)
    return;
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
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gis;
    gis.Pin = GPIO_PIN_7;
    gis.Mode = GPIO_MODE_OUTPUT_OD;
    gis.Pull = GPIO_PULLUP;
    gis.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gis);
    gis.Mode = GPIO_MODE_OUTPUT_PP;
    gis.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOB, &gis);
  }
  ZPin::write(PB5, 0);
  HAL_Delay(200);
  ZPin::write(PB5, 1);
  HAL_Delay(200);

  oled.init();
  oled.clear(0x00);

  UG_Init(&gui, pset, 128, 64);
  UG_SelectGUI(&gui);
//  UG_FillScreen(0x55);
  UG_FontSetHSpace(0);
  UG_FontSetVSpace(0);

  oled.write(vram, sizeof(vram));
//  while(1) { }

  DISP_Update();
}

void DISP_Update(void)
{
  UG_FillScreen(0x00);
  char buf[32] = {0};
  static int count = 0;
  ++count %= 2;

  UG_FontSelect (&FONT_12X16);
  UG_PutString(0, 0, (char*)charger.get_stat_str());
  UG_FontSelect (&FONT_12X24);
  float volt = 0, curr = 0;
  ADC_Get(&volt, &curr);
  auto vi = int(floor(volt));
  auto vf = int(floor((volt - vi) * 100));
  sprintf(buf, "%d.%02dV", vi, vf);
  UG_PutString(0, 16, buf);
  sprintf(buf, "%dmA", int(curr));
  UG_PutString(0, 40, buf);

  oled.write(vram, sizeof(vram));
}
