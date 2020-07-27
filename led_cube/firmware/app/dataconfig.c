#include "misc.h"
#include <string.h>
#include "platform.h"

static DataConfig_t g;
//bool flag_conf_updated = true;

DataConfig_t* DC_Get(void)
{
    return &g;
}

void DC_Config(void)
{
    FLASH_EEPROM_ReadData(0, &(g.config), sizeof(g.config));
    g.status.mode = DC_MODE_NORMAL;
    g.status.status = DC_STATUS_IDLE;
}

// @brief 更新配置
void DC_Update(void)
{
    unsigned char buf[sizeof(g.config)];

    FLASH_EEPROM_ReadData(0, buf, sizeof(g.config));
    if(memcmp(&(g.config), buf, sizeof(g.config)) != 0) {
        FLASH_EEPROM_WriteData(0, &(g.config), sizeof(g.config));
    }
}
