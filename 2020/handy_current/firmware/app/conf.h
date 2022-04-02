/// @file conf.h
/// @brief 
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R0.1
/// @date 2017-10-12
#ifndef _CONF_H
#define _CONF_H

#include "misc.h"

typedef struct {
    uint32_t baudrate;          // baudrate for uart
    uint8_t device_addr;        // device address for RS485
    uint8_t current_steps;
    uint8_t disp_brightness;
    uint8_t language;
} GlobalConfig_t;

void GlobalConfigUpdate(GlobalConfig_t *conf);
GlobalConfig_t* GlobalConfigRead(void);

#endif
