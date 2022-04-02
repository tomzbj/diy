#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#include "WM.h"

enum {
    DC_TX_MODE_CW,
    DC_TX_MODE_BPSK,
    DC_TX_MODE_FT8,
    DC_TX_MODE_MIN = DC_TX_MODE_CW,
    DC_TX_MODE_MAX = DC_TX_MODE_FT8
};
//enum { DC_SPK_ECHO_MODE_CW, DC_SPK_ECHO_MODE_RAW };
//enum { DC_CLI_ECHO_MODE_CW, DC_CLI_ECHO_MODE_ASCII };

// to config: cw wpm, cw carrier freq, psk31(fixed), ft8 carrier freq, echo volume, tx volume, time, mode
typedef struct {
    struct {
        int cw_wpm;
        int cw_carrier_freq;
        int ft8_carrier_freq;
        int echo_volume;
        int output_volume;
        int tx_mode;
        int dim;
    } cfg;    // could be written only by user, stored in eeprom
    struct {
        unsigned long ms;
    } data;     // could be written only by hardware
    struct {
        time_t time;
        unsigned char led_blinking;
        WM_HWIN hw_main, hw_cfg;
    } status;    // could be written both by user and hardware
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);
void DC_SetDefaults(void);

#endif /* _DC_H */
