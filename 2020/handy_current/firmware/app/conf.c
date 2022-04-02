/// @file conf.c
/// @brief 常用配置, 包括设备地址, 波特率, 当前选中通道, 4~20mA对应温度等
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R0.1
/// @date 2017-10-12
#include <string.h>
#include "conf.h"
#include "flash_eeprom.h"
#include "usart_f0.h"

const unsigned long GLOBAL_CONF_EEPROM_ADDR = 0UL;

static GlobalConfig_t g_conf;
bool flag_conf_updated = true;

/// @brief 读取配置
/// @param None
/// @retval 返回指向内部static结构的指针
GlobalConfig_t* GlobalConfigRead(void)
{
    if(flag_conf_updated == true) {
        EEPROM_ReadData(GLOBAL_CONF_EEPROM_ADDR, &g_conf, sizeof(GlobalConfig_t));
        flag_conf_updated = false;
    }
    return &g_conf;
}

/// @brief 写入配置
/// @param conf 指向配置信息结构体的指针
/// @retval None
static void GlobalConfigWrite(GlobalConfig_t* conf)
{
    EEPROM_WriteData(GLOBAL_CONF_EEPROM_ADDR, conf, sizeof(GlobalConfig_t));
}

/// @brief 更新配置
/// @param conf 指向配置信息结构体的指针
/// @retval None
void GlobalConfigUpdate(GlobalConfig_t* conf)
{
    GlobalConfig_t old_conf;
    EEPROM_ReadData(GLOBAL_CONF_EEPROM_ADDR, &old_conf, sizeof(GlobalConfig_t));
    // 新旧配置不同时才执行写入操作
    if (memcmp(&old_conf, &g_conf, sizeof(GlobalConfig_t)) != 0) {
        GlobalConfigWrite(&g_conf);
        flag_conf_updated = true;
    }
}
