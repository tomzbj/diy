#include "st7789.h"

TFT_ST7789::TFT_ST7789(int width, int height, int data_width, cbs_t cbs)    //
:
  TFT(width, height, data_width, cbs)
{
}

void TFT_ST7789::sleep(void)
{
  write_cmd(0x10);
}

void TFT_ST7789::init(void)
{
  _cbs.reset_f(0);
  _cbs.delay_us_f(50000);
  _cbs.reset_f(1);
  _cbs.delay_us_f(50000);

  //************* Start Initial Sequence **********//
  write_cmd(0x11);    //Sleep out
  _cbs.delay_us_f(120000UL);              //Delay 120ms
  //************* Start Initial Sequence **********//
  write_cmd(0x36);
  write_data8(0x00);    // hor!

  write_cmd(0x3a);
  write_data8(0x05);

  write_cmd(0xb2);
  write_data8(0x0c);
  write_data8(0x0c);
  write_data8(0x00);
  write_data8(0x33);
  write_data8(0x33);

  write_cmd(0xb7);
  write_data8(0x35);

  write_cmd(0xbb);
  write_data8(0x32);    //vcom=1.35v

  write_cmd(0xc2);
  write_data8(0x01);

  write_cmd(0xc3);
  write_data8(0x15);    //gvdd=4.8v  颜色深度

  write_cmd(0xc4);
  write_data8(0x20);    //vdv, 0x20:0v

  write_cmd(0xc6);
  write_data8(0x0f);    //0x0f:60hz

  write_cmd(0xd0);
  write_data8(0xa4);
  write_data8(0xa1);

  write_cmd(0xe0);
  write_data8(0xd0);
  write_data8(0x08);
  write_data8(0x0e);
  write_data8(0x09);
  write_data8(0x09);
  write_data8(0x05);
  write_data8(0x31);
  write_data8(0x33);
  write_data8(0x48);
  write_data8(0x17);
  write_data8(0x14);
  write_data8(0x15);
  write_data8(0x31);
  write_data8(0x34);

  write_cmd(0xe1);
  write_data8(0xd0);
  write_data8(0x08);
  write_data8(0x0e);
  write_data8(0x09);
  write_data8(0x09);
  write_data8(0x15);
  write_data8(0x31);
  write_data8(0x33);
  write_data8(0x48);
  write_data8(0x17);
  write_data8(0x14);
  write_data8(0x15);
  write_data8(0x31);
  write_data8(0x34);
  write_cmd(0x21);

  write_cmd(0x29);
  _cbs.delay_us_f(120000UL);              //delay 120ms

  clear(0x0000);
  /*
   uint16_t color = 0x1ff8;
   set_window(0, 0, 119, 119); for(int i = 0; i < 120 * 120; i++) { write_data8(color >> 8); write_data8(color); }
   set_window(0, 120, 120, 239); color = 0x1f00; for(int i = 0; i < 120 * 120; i++) { write_data8(color >> 8); write_data8(color); }
   set_window(120, 120, 239, 239); color = 0xe007; for(int i = 0; i < 120 * 120; i++) { write_data8(color >> 8); write_data8(color); }
   set_window(120, 0, 239, 119); color = 0xffff; for(int i = 0; i < 120 * 120; i++) { write_data8(color >> 8); write_data8(color); }
   */
}

void TFT_ST7789::set_window(int x1, int y1, int x2, int y2)
{
  /* 240x280 on ST7789 240x320 GRAM: column offset 20 (MADCTL=0x00). */
  y1 += 20;
  y2 += 20;
  write_cmd(0x2a);    //列地址设置
  write_data8(x1 >> 8);
  write_data8(x1);
  write_data8(x2 >> 8);
  write_data8(x2);
  write_cmd(0x2b);    //行地址设置
  write_data8(y1 >> 8);
  write_data8(y1);
  write_data8(y2 >> 8);
  write_data8(y2);
  write_cmd(0x2c);    //储存器写
}
