/* Author: tomzbj  Updated: 2025-01-17 Rev: 0.04 */

#ifndef _ZI2C_H
#define _ZI2C_H

//#include "misc.h"
#include "zpin.h"
#include <cstdint>

class ZI2C {
  public:
    ZI2C(zpin_t sda, zpin_t scl, uint8_t addr,
        void (*delay_us_f)(volatile int nus), int nus)    //
      : _sda(sda), _scl(scl), _addr(addr), _delay_us_f(delay_us_f), _nus(nus)
    {
    }

    // high-level ops
    void write_reg(uint8_t reg, uint8_t val) const;
    uint8_t read_reg(uint8_t reg) const;
    void write_reg(uint8_t reg, const uint8_t* data, int count) const;
    void read_reg(uint8_t reg, uint8_t* data, int count) const;
    void write_raw(const uint8_t* data, int count) const;
    void read_raw(uint8_t* data, int count) const;

    // callback to _delay_us

  private:
    const zpin_t _sda, _scl;
    const uint8_t _addr;
    void (*_delay_us_f)(volatile int nus);
    const int _retry_count = 10, _nus;

    // low-level ops
    inline void _setsda(int state) const
    {
      _delay_us_f(_nus);
      ZPin::write(_sda, state);
      _delay_us_f(_nus);
    }
    inline void _setscl(int state) const
    {
      _delay_us_f(_nus);
      ZPin::write(_scl, state);
      _delay_us_f(_nus);
    }
    inline int _getsda(void) const
    {
      _delay_us_f(_nus);
      return ZPin::read(_sda);
    }

    // mid-level ops
    void ack(void) const;
    void nack(void) const;
    void waitack(void) const;
    void start(void) const;
    void stop(void) const;
    uint8_t readbyte_ack(void) const;
    uint8_t readbyte_nack(void) const;
    uint8_t writebyte(uint8_t byte) const;
    void writebyte_waitack(uint8_t data) const;
};

#endif
