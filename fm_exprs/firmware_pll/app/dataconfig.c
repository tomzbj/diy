#include "dataconfig.h"

static DataConfig_t g;

void DC_Config(void)
{
    g.data.freq = 88000UL;      // in kHz
}
DataConfig_t* DC_Get(void)
{
    return &g;
}
