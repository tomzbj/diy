#include "misc.h"
#include "platform.h"

uint8_t vram[30];
UG_GUI gui;

static void pset(int16_t x, int16_t y, uint32_t c)
{
  if(x < 0 || x >= 20 || y < 0 || y >= 7)
    return;
  if(y % 2)
    x = (x + 10) % 20;
  y = (y * 4) % 7;
  if(x >= 10) {
    y += 3;
  }
  uint8_t yt = y % 7;
  if(c)
    vram[x % 20] |= (1 << yt);
  else
    vram[x % 20] &= ~(1 << yt);
}

void DISP_Config(void)
{
  UG_Init(&gui, pset, 30, 8);
  UG_SelectGUI(&gui);
  UG_FillScreen(0xf0);
  UG_FontSetHSpace(0);
  UG_FontSetVSpace(0);
  DISP_Update();
}

void DISP_Update(void)
{
  memset(vram, 0, sizeof(vram));
  UG_FillScreen(0x00);
  const UG_FONT* font = &FONT_6X8;
  UG_FontSelect(font);
  char buf[64];
  static int count = 0;
  ++count %= 150;

  sprintf(buf, "HELLO, WORLD.");
  UG_PutString(10 - abs(75 - count), 0, buf);
  extern class CHARLIE charlie;
  charlie.write(vram, sizeof(vram));
}
