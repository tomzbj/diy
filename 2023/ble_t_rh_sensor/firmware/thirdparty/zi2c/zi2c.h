#ifndef _ZI2C_H
#define _ZI2C_H

//#include "misc.h"
#include "zpin.h"

class zi2c {
public:
    zi2c(zpin_t sda, zpin_t scl, unsigned char addr,
        void (*delay_us_f)(volatile int nus));

    // high-level ops
    void write_reg(unsigned char reg, unsigned char val);
    unsigned char read_reg(unsigned char reg);
    void write_regs(unsigned char reg, const unsigned char* data, int count);
    void read_regs(unsigned char reg, unsigned char* data, int count);
    void read(unsigned char* data, int count);

    // callback to _delay_us

private:
    unsigned char _addr;
    int _retry_count;
    zpin_t _sda, _scl;
    void (*_delay_us_f)(volatile int nus);

    // low-level ops
    inline void _setsda(int state)
    {
        _delay_us_f(30);
        zpin_write(_sda, state);
        _delay_us_f(30);
    }
    inline void _setscl(int state)
    {
        _delay_us_f(30);
        zpin_write(_scl, state);
        _delay_us_f(30);
    }
    inline int _getsda(void)
    {
        _delay_us_f(30);
        return zpin_read(_sda);
    }

    // mid-level ops
    void ack(void);
    void nack(void);
    void waitack(void);
    void start(void);
    void stop(void);
    unsigned char readbyte_ack(void);
    unsigned char readbyte_nack(void);
    unsigned char read_write(unsigned char byte);
    void writebyte_waitack(unsigned char data);
};

#endif
