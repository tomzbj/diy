#ifndef _LM75_H
#define _LM75_H

#include "zi2c.h"

class LM75 {
  public:
    LM75(const ZI2C& i2c) : _i2c(i2c)
    {
    }
    int16_t read_temp(void)    // in 100 degrees
    {
      uint16_t val = 0;
      _i2c.read_reg(0x00, (uint8_t*)&val, 2);
//      return val;
      val = (val >> 8) | (val << 8);
      return ((int16_t)val * 25) / 64;
    }

  private:
    const ZI2C& _i2c;
};

#endif
