#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#include "misc.h"

#define DC_ADC_BUF_SIZE 64
#define DC_ADC_CHANNEL 1

enum {
    DC_STATE_SELECT, DC_STATE_MENU, DC_STATE_NORMAL, DC_STATE_EDIT
};

enum {
    DC_WAVEFORM_SINE, DC_WAVEFORM_RECTANGLE, DC_WAVEFORM_TRIANGLE,
    DC_WAVEFORM_SAWTOOTH, DC_WAVEFORM_MAX = DC_WAVEFORM_SAWTOOTH
};

enum {
    DC_AMPL_12DBM, DC_AMPL_9DBM, DC_AMPL_6DBM, DC_AMPL_3DBM, DC_AMPL_0DBM,
    DC_AMPL_MAX = DC_AMPL_0DBM
};

enum {
    DC_UNIT_HZ, DC_UNIT_KHZ, DC_UNIT_MHZ, DC_UNIT_AUTO,
    DC_UNIT_MAX = DC_UNIT_AUTO
};

typedef struct {
    struct {
        unsigned short adc_buf[64];
    } data;
    struct {
        unsigned long freq[4];
        int unit;
    } cfg;
    struct {
        int disp_state;
        int curr_preset;
        int ampl;
        int waveform;
    } state;
} DataConfig_t;

DataConfig_t* DC_Get(void);
void DC_Update(void);
void DC_SetDefaults(void);
void DC_Config(void);

#endif
