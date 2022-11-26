#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#include "misc.h"

#define DC_ADC_BUF_SIZE 64
#define DC_ADC_CHANNEL 3

typedef struct {
    struct {
        unsigned short adc_buf[DC_ADC_BUF_SIZE * DC_ADC_CHANNEL];
        unsigned short cur[DC_ADC_CHANNEL];
    } data;
    struct {
    } cfg;
    struct {
        int disp_state;
        int debug;
    } status;
} DataConfig_t;

DataConfig_t* DC_Get(void);
void DC_Update(void);
void DC_SetDefaults(void);
void DC_Config(void);

#endif
