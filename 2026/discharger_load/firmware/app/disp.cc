#include "misc.h"
#include "platform.h"
#include "ssd1306_i2c.h"
#include "disp.h"
#include "ugui.h"
#include <cmath>

auto nop = +[](volatile int n) {};
ZI2C i2c(PA6, PA7, 0x3c, nop, 0);
SSD1306 oled(i2c, _delay_us);
uint8_t vram[1024] = {0};
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

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_6;
  gis.Mode = GPIO_MODE_OUTPUT_OD;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pin = GPIO_PIN_7 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOA, &gis);

  ZPin::write(PA12, 0);
  HAL_Delay(200);
  ZPin::write(PA12, 1);
  HAL_Delay(200);

  oled.init();
  oled.clear(0x00);

  UG_Init(&gui, pset, 128, 64);
  UG_SelectGUI(&gui);
  UG_FontSetHSpace(0);
  UG_FontSetVSpace(0);

  print_log();
}

void DISP_Update(void)
{
  UG_FillScreen(0x00);
  char buf[32] = {0};
  static int count = 0;
  ++count %= 4;

  UG_FontSelect (&FONT_12X16);
  const char* name;
  float volt_terminal, curr_limit;
  meas.get_params(&name, &volt_terminal, &curr_limit);
  const char running_state[] = "-\\|/";
  if(meas.get_state() == meas.RUN)
    sprintf(buf, "%s%c", name, running_state[count]);
  else
    strcpy(buf, name);
  UG_PutString(0, 0, buf);

  UG_FontSelect (&FONT_8X16);

  sprintf(buf, "%.1fV %dmA", volt_terminal, int(floor(curr_limit)));
  UG_PutString(0, 16, buf);
  float volt, curr;
  ADC_Get(&volt, &curr);
  sprintf(buf, "%.2fV %dmA", volt, int(floor(curr)));
  UG_PutString(0, 32, buf);
  if(meas.is_dummy_load()) {
    sprintf(buf, "%.2fW", volt * curr / 1000.0f);
  }
  else {
    float capacity, energy;
    meas.get_capacity(&capacity, &energy);
    sprintf(buf, "%dmAh %dmWh", int(floor(capacity)), int(floor(energy)));
  }
  UG_PutString(0, 48, buf);

  oled.write(vram, sizeof(vram));
}
