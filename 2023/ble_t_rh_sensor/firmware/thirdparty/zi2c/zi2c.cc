#include "zi2c.h"

zi2c::zi2c(zpin_t sda, zpin_t scl, unsigned char addr,
    void (*delay_us_f)(volatile int nus))
{
    _sda = sda;
    _scl = scl;
    _addr = addr;
    _delay_us_f = delay_us_f;
    _retry_count = 500;
}

void zi2c::start(void)
{
    _setsda(1);
    _setscl(1);
    _setsda(0);
    _setscl(0);
}

void zi2c::stop(void)
{
    _setsda(0);
    _setscl(1);
    _setsda(1);
    _setscl(0);
}

void zi2c::ack(void)
{
    _setsda(0);
    _setscl(1);
    _setscl(0);
    _setsda(1);
}

void zi2c::nack(void)
{
    _setsda(1);
    _setscl(1);
    _setscl(0);
}

void zi2c::waitack(void)
{
    _setsda(1);
    _setscl(1);
    for(int n = _retry_count; n > 0 && _getsda(); n--);
    _setscl(0);
}

unsigned char zi2c::read_write(unsigned char byte)
{
    unsigned char ret = 0;
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

unsigned char zi2c::readbyte_nack(void)
{
    unsigned char data = read_write(0xff);
    nack();
    return data;
}

unsigned char zi2c::readbyte_ack(void)
{
    unsigned char data = read_write(0xff);
    ack();
    return data;
}

void zi2c::writebyte_waitack(unsigned char data)
{
    read_write(data);
    waitack();
}

void zi2c::write_reg(unsigned char reg, unsigned char val)
{
    start();
    writebyte_waitack(_addr << 1);
    writebyte_waitack(reg);
    writebyte_waitack(val);
    stop();
}

unsigned char zi2c::read_reg(unsigned char reg)
{
    return 0;
}

void zi2c::write_regs(unsigned char reg, const unsigned char* data, int count)
{
    start();
    writebyte_waitack(_addr << 1);
    writebyte_waitack(reg);
    for(int i = 0; i < count; i++)
        writebyte_waitack(*data++);
    stop();
}

void zi2c::read(unsigned char* data, int count)
{
    start();
    writebyte_waitack((_addr << 1) + 1);
    for(int i = 0; i < count; i++)
        *data++ = readbyte_ack();
    stop();
}

void zi2c::read_regs(unsigned char reg, unsigned char* data, int count)
{
}
