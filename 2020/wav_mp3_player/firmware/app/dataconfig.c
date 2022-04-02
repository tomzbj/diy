#include <string.h>
#include "misc.h"
#include "dataconfig.h"

static DataConfig_t g;

void DC_Config(void)
{
//    EEPROM_Read(0, &(g.config), sizeof(g.config));
//    g.status.state = STATE_DISP;
//    g.status.halt = 0;
//    g.status.count_halt = 0;
}

void DC_Update(void)
{
    /*    unsigned char buf[sizeof(g.config)];
     EEPROM_Read(0, buf, sizeof(g.config));
     if(memcmp(&(g.config), buf, sizeof(g.config)) != 0) {
     EEPROM_Write(0, &(g.config), sizeof(g.config));
     } */
}

DataConfig_t* DC_Get(void)
{
    return &g;
}
