#include "dataconfig.h"

static DataConfig_t g;

DataConfig_t* DC_Get(void)
{
    return &g;
}
