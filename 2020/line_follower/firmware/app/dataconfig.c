#include <string.h>
#include "platform.h"

static DataConfig_t g;

DataConfig_t* DC_Get(void)
{
    return &g;
}

void DC_Config(void)
{
    FLASH_EEPROM_ReadData(0, &(g.cfg), sizeof(g.cfg));
}

// @brief 更新配置
void DC_Update(void)
{
    unsigned char buf[sizeof(g.cfg)];

    FLASH_EEPROM_ReadData(0, buf, sizeof(g.cfg));
    if(memcmp(&(g.cfg), buf, sizeof(g.cfg)) != 0) {
        FLASH_EEPROM_WriteData(0, &(g.cfg), sizeof(g.cfg));
    }
}
