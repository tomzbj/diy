#include "gc9307.h"

TFT_GC9307::TFT_GC9307(int width, int height, int data_width, cbs_t cbs)    //
: TFT(width, height, data_width, cbs)
{
}

void TFT_GC9307::sleep(void)
{
  write_cmd(0x10);
}

void TFT_GC9307::init(void)
{
  _cbs.reset_f(0);
  _cbs.delay_us_f(5000);
  _cbs.reset_f(1);
  _cbs.delay_us_f(5000);

  write_cmd(0x11);    //Sleep out
  _cbs.delay_us_f(12000UL);              //Delay 120ms
  write_cmd(0xfe);
  write_cmd(0xef);                                          //
  write_cmd(0x36);
  write_data8(0xe8);

  write_cmd(0x3a);
  write_data8(0x05);

  write_cmd(0x86);
  write_data8(0x98);
  write_cmd(0x89);
  write_data8(0x03);
  write_cmd(0x8b);
  write_data8(0x80);
  write_cmd(0x8d);
  write_data8(0x33);
  write_cmd(0x8e);
  write_data8(0x0f);
  write_cmd(0xe8);
  write_data8(0x12);
  write_data8(0x00);
  //source ps
  write_cmd(0xff);
  write_data8(0x62);
  write_cmd(0x99);
  write_data8(0x3e);
  write_cmd(0x9d);
  write_data8(0x4b);
  write_cmd(0x98);
  write_data8(0x3e);
  write_cmd(0x9c);
  write_data8(0x4b);
  write_cmd(0xc3);
  write_data8(0x27);
  write_cmd(0xc4);
  write_data8(0x18);
  write_cmd(0xc9);
  write_data8(0x1F);
  //end vreg
  write_cmd(0xF0);
  write_data8(0x8F);
  write_data8(0x16);
  write_data8(0x06);
  write_data8(0x06);
  write_data8(0x06);
  write_data8(0x3C);
  write_cmd(0xF2);
  write_data8(0x8F);
  write_data8(0x13);
  write_data8(0x06);
  write_data8(0x06);
  write_data8(0x07);
  write_data8(0x3B);
  write_cmd(0xF1);
  write_data8(0x52);
  write_data8(0xBC);
  write_data8(0x8f);
  write_data8(0x35);
  write_data8(0x38);
  write_data8(0x4F);
  write_cmd(0xF3);
  write_data8(0x54);
  write_data8(0xBC);
  write_data8(0x8f);
  write_data8(0x33);
  write_data8(0x3C);
  write_data8(0x4F);
  write_cmd(0x35);
  write_data8(0x00);
//  write_cmd(0x21);
  write_cmd(0x44);
  write_data8(0x00);
  write_data8(0x0a);
  write_cmd(0x11);
  _cbs.delay_us_f(120000UL);
  write_cmd(0x29);
}

void TFT_GC9307::set_window(int x1, int y1, int x2, int y2)
{
  y1 += 34;
  y2 += 34;
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
