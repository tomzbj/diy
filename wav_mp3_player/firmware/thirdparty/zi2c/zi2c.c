#include "misc.h"
#include "zi2c.h"

static void start(zi2c_t* zi2c)
{
    zi2c->setsda_f(1);
    zi2c->setscl_f(1);
    zi2c->setsda_f(0);
    zi2c->setscl_f(0);
}

static void stop(zi2c_t* zi2c)
{
    zi2c->setsda_f(0);
    zi2c->setscl_f(1);
    zi2c->setsda_f(1);
    zi2c->setscl_f(0);
}

static void ack(zi2c_t* zi2c)
{
    zi2c->setsda_f(0);
    zi2c->setscl_f(1);
    zi2c->setscl_f(0);
    zi2c->setsda_f(1);
}

static void nack(zi2c_t* zi2c)
{
    zi2c->setsda_f(1);
    zi2c->setscl_f(1);
    zi2c->setscl_f(0);
}

static void waitack(zi2c_t* zi2c)
{
    zi2c->setsda_f(1);
    zi2c->setscl_f(1);
    ( {  for(int n = 600; n > 0 && zi2c->getsda_f(); n--);});
    zi2c->setscl_f(0);
}

static unsigned char writebyte(zi2c_t* zi2c, unsigned char byte)
{
    unsigned char ret = 0;
    for(int i = 0; i < 8; i++) {
        ret <<= 1;
        (byte & 0x80) ? (zi2c->setsda_f(1)) : (zi2c->setsda_f(0));
        zi2c->setscl_f(1);
        if(zi2c->getsda_f())
            ret |= 0x1;
        zi2c->setscl_f(0);
        byte <<= 1;
    }
    return ret;
}

static unsigned char read_byte_nack(zi2c_t* zi2c)
{
    unsigned char data = writebyte(zi2c, 0xff);
    nack(zi2c);
    return data;
}

static unsigned char read_byte_ack(zi2c_t* zi2c)
{
    unsigned char data = writebyte(zi2c, 0xff);
    ack(zi2c);
    return data;
}

static void write_byte_waitack(zi2c_t* zi2c, unsigned char data)
{
    writebyte(zi2c, data);
    waitack(zi2c);
}

void zi2c_init(zi2c_t* zi2c)
{
    zi2c->start_f = start;
    zi2c->stop_f = stop;
    zi2c->readbyte_ack_f = read_byte_ack;
    zi2c->readbyte_nack_f = read_byte_nack;
    zi2c->writebyte_f = write_byte_waitack;
}

