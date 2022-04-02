#ifndef _CONF_H
#define _CONF_H

#include "misc.h"

enum {
    DC_DISP_STATE_NORMAL, DC_DISP_STATE_MENU, DC_DISP_STATE_SELECT
};

enum {
    DC_DISCHARGE_STATE_OFF, DC_DISCHARGE_STATE_ON
};

typedef struct {
    struct {
        signed short discharge_curr;
        signed short end_volt;
    } cfg;    // could be written only by user, stored in eeprom
    struct {
        float curr, vbat1, vbat2;
        float total_capacity;
    } data;     // could be written only by hardware
    struct {
        int disp_state;
        int discharge_state;
    } status;    // could be written both by user and hardware
} DataConfig_t;

DataConfig_t* DC_Get(void);
void DC_Config(void);
void DC_Update(void);

#endif
