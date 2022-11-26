#include "platform.h"
#include "misc.h"

const unsigned long DC_EEPROM_ADDR = 0UL;

static DataConfig_t g;

DataConfig_t* DC_Get(void)
{
    return &g;
}

void DC_SetDefaults(void)
{
    DC_Update();
}

void DC_Config(void)
{
    FLASH_EEPROM_ReadData(0, &(g.cfg), sizeof(g.cfg));
}

void DC_Update(void)
{
    unsigned char buf[sizeof(g.cfg)];

    FLASH_EEPROM_ReadData(DC_EEPROM_ADDR, buf, sizeof(g.cfg));
    if(memcmp(&(g.cfg), buf, sizeof(g.cfg)) != 0) {
        FLASH_EEPROM_WriteData(DC_EEPROM_ADDR, &(g.cfg), sizeof(g.cfg));
    }
}
