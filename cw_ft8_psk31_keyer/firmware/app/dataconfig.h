#ifndef _CONF_H
#define _CONF_H

#include "misc.h"

enum {
    DC_TX_MODE_CW, DC_TX_MODE_BPSK, DC_TX_MODE_FT8
};
enum {
    DC_SPK_ECHO_MODE_CW, DC_SPK_ECHO_MODE_RAW
};
enum {
    DC_CLI_ECHO_MODE_CW, DC_CLI_ECHO_MODE_ASCII
};

typedef struct {
    struct {
        int spk_echo_freq;
        int psk_carrier_freq;
        int cw_wpm;
        int psk_baudrate;
        unsigned char spk_echo_mode;
        unsigned char spk_echo_state;
        unsigned char cli_echo_state;
        unsigned char cli_echo_mode;
        unsigned char tx_mode;
        signed char time_zone;
    } cfg;    // could be written only by user, stored in eeprom
    struct {
        unsigned long ms;
    } data;     // could be written only by hardware
    struct {
        time_t time;
        unsigned char led_blinking;
    } status;    // could be written both by user and hardware
} DataConfig_t;

DataConfig_t* DC_Get(void);
void DC_Config(void);
void DC_Update(void);
void DC_SetDefaults(void);

#endif
