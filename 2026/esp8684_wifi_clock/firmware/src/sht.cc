#include "sht.h"
#include "platform.h"
#include "zi2c_arduino.h"
#include "wht20.h"

auto _delay_us = [](volatile int nus) {delayMicroseconds(nus);};
ZI2C i2c(10, 18, 0x38, _delay_us, 3);    // P10 as SDA, P18 as SCL
WHT20 sensor(i2c);

void SHT_Config(void)
{
  pinMode(10, OUTPUT_OPEN_DRAIN);
  pinMode(18, OUTPUT_OPEN_DRAIN);
  sensor.init();
}
