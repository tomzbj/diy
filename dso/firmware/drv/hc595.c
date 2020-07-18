#include "hc595.h"

static hc595_cfg_t g;

void HC595_Config(hc595_cfg_t* cfg)
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
    g.delay_f(1);
    for(int i = 0; i < nbytes; i++) {
        g.writebyte_f(*(unsigned char*)buf);
        buf++;
    }
    g.delay_f(1);
    g.cs_f(1);
}
