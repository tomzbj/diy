#include "st7305.h"

void ST7305::set_window(void)
{
  write_cmd(0x2a);    //列地址设置
  write_data(0x16);
  write_data(0x26);
  write_cmd(0x2b);    //行地址设置
  write_data(0x00);
  write_data(0x63);
  write_cmd(0x2c);    //储存器写
}

void ST7305::init(void)
{
  _cbs.reset_f(0);                 //复位
  _cbs.delay_us_f(10000);
  _cbs.reset_f(1);                 //复位
  _cbs.delay_us_f(30000);    // _cbs.delay_us_f 300ms

  write_cmd(0xD6);    //OTP Load Control
  write_data(0x17);
  write_data(0x02);

  write_cmd(0xD1);    //Auto Power Control
  write_data(0x01);

  write_cmd(0xC0);    //Gate Voltage Setting VGH=12V ; VGL=-5V
  write_data(0x08);
  write_data(0x02);

  write_cmd(0xC1);    //VSH Setting
  write_data(0x19);    //
  write_data(0x19);    //
  write_data(0x19);    //
  write_data(0x19);    //

  write_cmd(0xC2);    //VSL Setting VSL=0
  write_data(0x31);
  write_data(0x31);
  write_data(0x31);
  write_data(0x31);

  write_cmd(0xC4);
  write_data(0x19);
  write_data(0x19);
  write_data(0x19);
  write_data(0x19);

  write_cmd(0xC5);
  write_data(0x27);
  write_data(0x27);
  write_data(0x27);
  write_data(0x27);

  write_cmd(0xD8);
  write_data(0xa6);
  write_data(0xe9);

  write_cmd(0xB2);    //Frame Rate Control
  write_data(0X12);    //HPM=32hz ; LPM=1hz

  write_cmd(0xB3);    //Update Period Gate EQ Control in HPM
  write_data(0XE5);
  write_data(0XF6);
  write_data(0X05);    //HPM EQ Control
  write_data(0X46);
  write_data(0X77);
  write_data(0X77);
  write_data(0X77);
  write_data(0X77);
  write_data(0X76);
  write_data(0X45);

  write_cmd(0xB4);    //Update Period Gate EQ Control in LPM
  write_data(0X05);    //LPM EQ Control
  write_data(0X46);
  write_data(0X77);
  write_data(0X77);
  write_data(0X77);
  write_data(0X77);
  write_data(0X76);
  write_data(0X45);

  write_cmd(0x62);    //Gate Timing Control
  write_data(0X32);
  write_data(0X03);
  write_data(0X1F);

  write_cmd(0xB7);    //Source EQ Enable
  write_data(0X13);

  write_cmd(0xB0);    // gate line setting
  write_data(0x32);    // 200 line

  write_cmd(0x11);    //Sleep out
  _cbs.delay_us_f(100000);    // _cbs.delay_us_f 100ms

  write_cmd(0xC9);    //Source Voltage Select
  write_data(0X00);    //VSHP1; VSLP1 ; VSHN1 ; VSLN1

  write_cmd(0x36);    //Memory Data Access Control
  write_data(0x48);    // 重要！显示方向！

  write_cmd(0x3A);    //Data Format Select 4 write for 24 bit
  write_data(0x11);

  write_cmd(0xB9);    //Source Setting
  write_data(0x20);

  write_cmd(0xB8);    //Panel Setting Frame inversion
  write_data(0x29);

  write_cmd(0x35);    //TE
  write_data(0X00);    //

  write_cmd(0xD0);    //Auto power dowb
  write_data(0XFF);    //

  write_cmd(0x39);         //LPM
  write_cmd(0x29);    //Display on
}
