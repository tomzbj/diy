#include "zi2c.h"
#include "lis3dhtr.h"

static zi2c_t g;

enum {
    REG_CTRL_REG1 = 0x20
};
#define  CMD_WRITE (g.addr << 1)
#define  CMD_READ ((g.addr << 1) + 1)

static unsigned char ReadReg(unsigned char reg)
{
    unsigned char val;
    g.start_f(&g);
    g.writebyte_f(&g, CMD_WRITE);
    g.writebyte_f(&g, reg);
    g.start_f(&g);
    g.writebyte_f(&g, CMD_READ);
    val = g.readbyte_nack_f(&g);
    g.stop_f(&g);
    return val;
}

static void ReadRegs(unsigned char reg, unsigned char buf[], int nregs)
{
    int i;
    g.start_f(&g);
    g.writebyte_f(&g, CMD_WRITE);
    g.writebyte_f(&g, reg | 0x80);
    g.start_f(&g);
    g.writebyte_f(&g, CMD_READ);
    for(i = 0; i < nregs - 1; i++)
        buf[i] = g.readbyte_ack_f(&g);
    buf[i] = g.readbyte_nack_f(&g);
    g.stop_f(&g);
}

static void WriteReg(unsigned char reg, unsigned char val)
{
    g.start_f(&g);
    g.writebyte_f(&g, CMD_WRITE);
    g.writebyte_f(&g, reg);
    g.writebyte_f(&g, val);
    g.stop_f(&g);
}

lis3_acclr_t LIS3DHTR_ReadAcclr(void)
{
    lis3_acclr_t a;
    unsigned char buf[6];
    ReadRegs(0x28, buf, 6);
    a.x = buf[0] + buf[1] * 256;
    a.y = buf[2] + buf[3] * 256;
    a.z = buf[4] + buf[5] * 256;
    return a;
}

void LIS3DHTR_Init(zi2c_t* obj)
{
    g = *obj;
    ReadReg(0x20);
    WriteReg(REG_CTRL_REG1, 0x27);
}
