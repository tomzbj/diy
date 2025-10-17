#ifndef _SSD1306_I2C_H
#define _SSD1306_I2C_H

#include "zi2c.h"
#include "misc.h"

class SSD1306 {
  public:
    SSD1306(ZI2C& i2c, void (*delay_us_f)(volatile int nus));
    void init(void);
    void display_on(void);
    void display_off(void);
    void clear(uint8_t data);
    void write(uint8_t* buf, int size);
    void set_brightness(uint8_t brightness);
    void set_contrast(uint8_t contrast);

  private:
    inline void write_cmd(uint8_t cmd)
    {
      _i2c.write_reg(0x00, cmd);
    }
    inline void write_data(uint8_t* data, int count)
    {
      _i2c.write_reg(0x40, data, count);
    }

    ZI2C& _i2c;
    void (*_delay_us_f)(volatile int nus);
};

#endif
