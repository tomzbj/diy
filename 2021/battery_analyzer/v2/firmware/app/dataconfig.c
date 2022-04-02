#include "misc.h"
#include "platform.h"

static DataConfig_t g;

DataConfig_t* DC_Get(void)
{
    return &g;
}

void DC_Config(void)
{
    FLASH_EEPROM_ReadData(0, &(g.cfg), sizeof(g.cfg));

    g.status.disp_state = DC_DISP_STATE_NORMAL;
    g.status.discharge_state = DC_DISCHARGE_STATE_OFF;
}

void DC_Update(void)
{
_dbg();
    unsigned char buf[sizeof(g.cfg)];
    FLASH_EEPROM_ReadData(0, buf, sizeof(g.cfg));
    if(memcmp(&(g.cfg), buf, sizeof(g.cfg)) != 0) {
_dbg();
        FLASH_EEPROM_WriteData(0, &(g.cfg), sizeof(g.cfg));
    }
}
