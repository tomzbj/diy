#include "st7789.h"

TFT_ST7789::TFT_ST7789(int width, int height, int data_width, cbs_t cbs)    //
: TFT(width, height, data_width, cbs)
{
}

void TFT_ST7789::sleep(void)
{
  write_cmd(0x10);
}

void TFT_ST7789::set_dir(int dir)
{
  _dir = dir;
  printf("%d\n", dir);
  uint8_t vals[] = {0x60, 0xc0, 0xa0, 0x00};
  write_cmd(0x36);
  write_data8(vals[dir]);
}

void TFT_ST7789::init(void)
{
  _cbs.reset_f(0);
  _cbs.delay_us_f(500000);
  _cbs.reset_f(1);
  _cbs.delay_us_f(500000);

  //************* Start Initial Sequence **********//
  write_cmd(0x11);    //Sleep out
  _cbs.delay_us_f(120000UL);    //Delay 120ms
  //************* Start Initial Sequence **********//
  write_cmd(0x36);
  write_data8(0x60);    // hor!

  write_cmd(0x3a);
  write_data8(0x05);

  write_cmd(0xb2);
  write_data8(0x0c);
  write_data8(0x0c);
  write_data8(0x00);
  write_data8(0x33);
  write_data8(0x33);

  write_cmd(0xb7);
//  write_data8(0x35);
  write_data8(0x71);

  write_cmd(0xbb);
//  write_data8(0x32);    //vcom=1.35v
  write_data8(0x3b);    //vcom=1.35v

  write_cmd(0xc0);
  write_data8(0x2c);

  write_cmd(0xc2);
  write_data8(0x01);

  write_cmd(0xc3);
//  write_data8(0x15);    //gvdd=4.8v  颜色深度
  write_data8(0x13);    //gvdd=4.8v  颜色深度

  write_cmd(0xc4);
  write_data8(0x20);    //vdv, 0x20:0v

  write_cmd(0xc6);
  write_data8(0x0f);    //0x0f:60hz

  write_cmd(0xd0);
  write_data8(0xa4);
  write_data8(0xa1);

  write_cmd(0xd6);
  write_data8(0xa1);

  write_cmd(0xe0);
  write_data8(0xd0);
  write_data8(0x08);
  write_data8(0x0a);
  write_data8(0x0d);
  write_data8(0x0b);
  write_data8(0x07);
  write_data8(0x21);
  write_data8(0x33);
  write_data8(0x39);
  write_data8(0x39);
  write_data8(0x16);
  write_data8(0x16);
  write_data8(0x1f);
  write_data8(0x3c);

  write_cmd(0xe1);
  write_data8(0xd0);
  write_data8(0x00);
  write_data8(0x03);
  write_data8(0x01);
  write_data8(0x00);
  write_data8(0x10);
  write_data8(0x21);
  write_data8(0x32);
  write_data8(0x38);
  write_data8(0x16);
  write_data8(0x14);
  write_data8(0x14);
  write_data8(0x20);
  write_data8(0x3d);
  write_cmd(0x21);

  write_cmd(0x29);
  _cbs.delay_us_f(120000UL);    //delay 120ms

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
  if(_dir == 1) {
    y1 += 80;
    y2 += 80;
  }
  if(_dir == 2) {
    x1 += 80;
    x2 += 80;
  }
  write_cmd(0x2a);    //列地址设置
  write_data8(x1 >> 8);
  write_data8(x1);
  write_data8(x2 >> 8);
  write_data8(x2);
  write_cmd(0x2b);    //行地址设置
  write_data8(y1 >> 8);
  write_data8(y1);
//    write_data8(y1 + 20);
//    write_data8(y2 + 20);
  write_data8(y2 >> 8);
  write_data8(y2);
  write_cmd(0x2c);    //储存器写
}
