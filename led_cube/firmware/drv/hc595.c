#include "hc595.h"

static hc595_cfg_t g;

void HC595_Init(hc595_cfg_t* cfg)
{
    g = *cfg;
}

void HC595_WriteByte(unsigned char data)
{
    g.cs_f(0);
    g.delay_f(1);
    g.writebyte_f(data);
    g.delay_f(1);
    g.cs_f(1);
}

void HC595_Write(void* buf, int nbytes)
{
    g.cs_f(0);
    g.delay_f(10);
    g.fastwrite_f(buf, nbytes);
    g.delay_f(10);
    g.cs_f(1);
}
