#include "misc.h"
#include "platform.h"

static DataConfig_t g;

void DC_Config(void)
{
    FLASH_EEPROM_ReadData(0, &(g.cfg), sizeof(g.cfg));
    g.status.t_sp = 30.0;
    g.status.debug = 0;
}

void DC_Update(void)
{
    unsigned char buf[sizeof(g.cfg)];
    FLASH_EEPROM_ReadData(0, buf, sizeof(g.cfg));
    if(memcmp(&(g.cfg), buf, sizeof(g.cfg)) != 0) {
        FLASH_EEPROM_WriteData(0, &(g.cfg), sizeof(g.cfg));
    }
}

DataConfig_t* DC_Get(void)
{
    return &g;
}
