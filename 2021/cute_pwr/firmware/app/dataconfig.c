#include "misc.h"
#include "platform.h"

static DataConfig_t gdc;

void DC_Config(void)
{
    FLASH_EEPROM_ReadData(0, &(gdc.cfg), sizeof(gdc.cfg));
    gdc.status.v_dcdc = 0;
    gdc.status.v_sp = 0;
    gdc.status.i_sp = 0;
}

void DC_Update(void)
{
    unsigned char buf[sizeof(gdc.cfg)];

    FLASH_EEPROM_ReadData(0, buf, sizeof(gdc.cfg));
    if(memcmp(&(gdc.cfg), buf, sizeof(gdc.cfg)) != 0) {
        FLASH_EEPROM_WriteData(0, &(gdc.cfg), sizeof(gdc.cfg));
    }
}

DataConfig_t* DC_Get(void)
{
    return &gdc;
}
