#ifndef _CONF_H
#define _CONF_H

#include "misc.h"

enum {
    DC_MODE_NORMAL, DC_MODE_FIND_OFF, DC_MODE_FIND_ON
};

enum {
    DC_STATUS_IDLE, DC_STATUS_CHECKING
};

typedef struct {
    struct {
        int debug;
    } config;    // could be written only by user, stored in eeprom
    struct {
        int on_count, off_count;
        int evt_pin1, evt_pin2;
    } data;     // could be written only by hardware
    struct {
        int mode;
        int status;
    } status;    // could be written both by user and hardware
} DataConfig_t;

DataConfig_t* DC_Get(void);
const char* DC_GetRangeStr(int range);
const char* DC_GetConfigSWStr(unsigned char sw);
void DC_Config(void);
void DC_Update(void);

#endif
