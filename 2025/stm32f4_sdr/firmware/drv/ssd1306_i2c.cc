#include "SSD1306_i2c.h"

#define  START_X 0

enum {
  CMD_DISP_OFF = 0xae, CMD_DISP_ON = 0xaf, CMD_STARTLINE_0 = 0x40,
  CMD_SET_CONTRAST = 0x81, CMD_SEG_REMAP_COL127_SEG0 = 0xa1,
  CMD_COM_SCAN_DIR_REMAPPED = 0xc8, CMD_DISP_NORMAL = 0xa6,
  CMD_DISP_INVERSE = 0xa7, CMD_MULTIPLEX_RATIO = 0xa8, CMD_DISP_OFFSET = 0xd3,
  CMD_DISP_CLK_DIVR_FREQ = 0xd5, CMD_PRECHARGE_PERIOD = 0xd9,
  CMD_COM_PINS_HW_CONF = 0xda, CMD_SET_VCOMH_DESELECT_LEVEL = 0xdb,
  CMD_SET_MEM_ADDR_MODE = 0x20, CMD_SET_CHARGE_PUMP_ENABLE_1 = 0x8d,
  CMD_SET_CHARGE_PUMP_ENABLE_2 = 0x14, CMD_SET_ENTIRE_DISP_ON = 0xa4
};

SSD1306::SSD1306(ZI2C& i2c, void (*delay_us_f)(volatile int nus))    //
: _i2c(i2c), _delay_us_f(delay_us_f)
{
}

void SSD1306::init(void)
{
  write_cmd(0xae);    // display off
  write_cmd(0x00);    // 00h~0fh: lower column start address for page addressing mode
  write_cmd(0x10);    // 10h~1fh: higher column start address for page addressing mode
  write_cmd(0x40);    // 40h~7fh: start line
  write_cmd(0xb0);    // b0h~b7h: page start address
  write_cmd(0x81);    // 81h: contrast control
  write_cmd(0xff);    // contrast value
//    write_cmd(0x20);    // contrast value
  write_cmd(0xa1);    // a0/a1h: column address 127 mapped to seg0
  write_cmd(0xa6);    // a6/a7h: normal/invert display
  write_cmd(0xa8);    // a8h: multiplex ratio
  write_cmd(0x3f);    // ratio, 16 to 63
  write_cmd(0xc8);    // c0h/c8h: com out scan direction
  write_cmd(0xa0);    // a0h/a1h:
  write_cmd(0xd3);    // d3h: display offset
  write_cmd(0x00);    // offset value
  write_cmd(0xd5);    // d5h: display clock divide ratio & oscillator frequency
  write_cmd(0xf0);    // div ratio value & frequency value
  write_cmd(0xd9);    // d9h: precharge period
  write_cmd(0xf1);    // precharge value??
  write_cmd(0xda);    // dah: com pins hardware configuration
  write_cmd(0x20);    // configuration value  12 for 12864, 22 for 12832
  write_cmd(0xdb);    // dbh: vcomh deselect level
  write_cmd(0x49);    // vcomh deselect level value
  write_cmd(0x8d);    // ???
  write_cmd(0x14);    // ???
  write_cmd(0x20);
  write_cmd(0x02);
  write_cmd(0xaf);    // display on

  _delay_us_f(100000UL);
  clear(0x00);
//    SSD1306_SetXy(0, 0);
}

void SSD1306::display_on(void)
{
  write_cmd(0X8D);    //SET DCDC
  write_cmd(0X14);    //DCDC ON
  write_cmd(0XAF);    //DISPLAY ON
}
void SSD1306::display_off(void)
{
  write_cmd(0X8D);    //SET DCDC
  write_cmd(0X10);    //DCDC OFF
  write_cmd(0XAE);    //DISPLAY OFF
}

void SSD1306::clear(uint8_t data)
{
  uint8_t buf[128];

  for(size_t i = 0; i < sizeof(buf); i++)
    buf[i] = data;
  for(int i = 0; i < 4; i++) {
    write_cmd(0xb0 + i);
    write_cmd(0x00);
    write_cmd(0x10 | START_X >> 4);
    write_cmd(START_X & 0xf);
    write_data(buf, sizeof(buf));
  }
}

void SSD1306::write(uint8_t* buf, int size)
{
  for(int i = 0; i < 4; i++) {
    write_cmd(0xb0 + i);
    write_cmd(0x00);
    write_cmd(0x10 | START_X >> 4);
    write_cmd(START_X & 0x0f);
    write_data(buf, 128);
    buf += 128;
  }
}

void SSD1306::set_brightness(uint8_t brightness)
{
  if(brightness > 0xf)
    brightness = 0xf;
  brightness = (brightness << 4) | brightness;
  write_cmd(CMD_PRECHARGE_PERIOD);
  write_cmd(brightness);
}

void SSD1306::set_contrast(uint8_t contrast)
{
  write_cmd(CMD_SET_CONTRAST);
  write_cmd(contrast);
}
