/* Author: tomzbj  Updated: 2025-01-17 Rev: 0.04 */

#include "ZI2C.h"

void ZI2C::start(void) const
{
  _setsda(1);
  _setscl(1);
  _setsda(0);
  _setscl(0);
}

void ZI2C::stop(void) const
{
  _setsda(0);
  _setscl(1);
  _setsda(1);
  _setscl(0);
}

void ZI2C::ack(void) const
{
  _setsda(0);
  _setscl(1);
  _setscl(0);
  _setsda(1);
}

void ZI2C::nack(void) const
{
  _setsda(1);
  _setscl(1);
  _setscl(0);
}

void ZI2C::waitack(void) const
{
  _setsda(1);
  _setscl(1);
  for(int n = _retry_count; n > 0 && _getsda(); n--);
  _setscl(0);
}

uint8_t ZI2C::writebyte(uint8_t byte) const
{
  uint8_t ret = 0;
  for(int i = 0; i < 8; i++) {
    ret <<= 1;
    (byte & 0x80) ? (_setsda(1)) : (_setsda(0));
    _setscl(1);
    if(_getsda())
      ret |= 0x1;
    _setscl(0);
    byte <<= 1;
  }
  return ret;
}

uint8_t ZI2C::readbyte_nack(void) const
{
  uint8_t data = writebyte(0xff);
  nack();
  return data;
}

uint8_t ZI2C::readbyte_ack(void) const
{
  uint8_t data = writebyte(0xff);
  ack();
  return data;
}

void ZI2C::writebyte_waitack(uint8_t data) const
{
  writebyte(data);
  waitack();
}

void ZI2C::write_reg(uint8_t reg, uint8_t val) const
{
  write_reg(reg, &val, 1);
}

uint8_t ZI2C::read_reg(uint8_t reg) const
{
  uint8_t val;
  read_reg(reg, &val, 1);
  return val;
}

void ZI2C::write_reg(uint8_t reg, const uint8_t* data, int count) const
{
  start();
  writebyte_waitack(_addr << 1);
  writebyte_waitack(reg);
  for(int i = 0; i < count; i++)
    writebyte_waitack(*data++);
  stop();
}

void ZI2C::write_raw(const uint8_t* data, int count) const
{
  start();
  writebyte_waitack(_addr << 1);
  for(int i = 0; i < count; i++)
    writebyte_waitack(*data++);
  stop();
}

void ZI2C::read_raw(uint8_t* data, int count) const
{
  start();
  writebyte_waitack((_addr << 1) + 1);
  for(int i = 0; i < count; i++) {
    if(i < count - 1)
      *data = readbyte_ack();
    else
      *data = readbyte_nack();
    data++;
  }
  stop();
}
void ZI2C::read_reg(uint8_t reg, uint8_t* data, int count) const
{
  start();
  writebyte_waitack(_addr << 1);
  writebyte_waitack(reg);
  start();
  writebyte_waitack((_addr << 1) + 1);
  //    writebyte_waitack(reg);
  for(int i = 0; i < count; i++) {
    if(i < count - 1)
      *data = readbyte_ack();
    else
      *data = readbyte_nack();
    data++;
  }
  stop();
}
