#include "misc.h"
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

void DC_Update(void)
{
    unsigned char buf[sizeof(g.cfg)];

    FLASH_EEPROM_ReadData(0, buf, sizeof(g.cfg));
    if(memcmp(&(g.cfg), buf, sizeof(g.cfg)) != 0) {
        FLASH_EEPROM_WriteData(0, &(g.cfg), sizeof(g.cfg));
    }
}

void DC_SetDefaults(void)
{
    g.cfg.psk_carrier_freq = 500;
    g.cfg.cw_wpm = 20;
    g.cfg.tx_mode = DC_TX_MODE_CW;

    g.cfg.spk_echo_freq = 500;
    g.cfg.spk_echo_state = 1;
    g.cfg.spk_echo_mode = DC_SPK_ECHO_MODE_CW;

    g.cfg.cli_echo_state = 1;
    g.cfg.cli_echo_mode = DC_CLI_ECHO_MODE_ASCII;
}
