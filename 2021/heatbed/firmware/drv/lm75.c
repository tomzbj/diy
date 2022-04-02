#include "zi2c.h"
#include "lm75.h"

static zi2c_t g;

#define  CMD_WRITE (g.addr << 1)
#define  CMD_READ ((g.addr << 1) + 1)

static signed short ReadReg(unsigned char reg)
{
    signed short val;
    g.start_f(&g);
    g.writebyte_f(&g, CMD_WRITE);
    g.writebyte_f(&g, reg);
    g.start_f(&g);
    g.writebyte_f(&g, CMD_READ);
    val = g.readbyte_ack_f(&g);
    val <<= 8;
    val |= g.readbyte_nack_f(&g);
    g.stop_f(&g);
    return val;
}

void LM75_Init(zi2c_t* obj)
{
    g = *obj;
}

signed short LM75_ReadTemp(void)
{
    return ReadReg(0x00);
}

