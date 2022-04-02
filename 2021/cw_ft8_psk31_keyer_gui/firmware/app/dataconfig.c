#include "misc.h"
#include "platform.h"

static DataConfig_t g;

void DC_Config(void)
{
    FLASH_EEPROM_ReadData(0, &(g.cfg), sizeof(g.cfg));
// cw_wpm cw_carrier_freq ft8_carrier_freq echo_volume output_volume tx_mode
    typeof(g.cfg)* p = &(g.cfg);
    if(p->cw_wpm < 10 || p->cw_wpm > 120)
        p->cw_wpm = 20;
    if(p->cw_carrier_freq < 200 || p->cw_carrier_freq > 1000)
        p->cw_carrier_freq = 500;
    if(p->ft8_carrier_freq < 200 || p->ft8_carrier_freq > 1000)
        p->ft8_carrier_freq = 500;
    if(p->echo_volume < 0 || p->echo_volume > 10)
        p->echo_volume = 5;
    if(p->output_volume < 0 || p->output_volume > 10)
        p->output_volume = 5;
    if(p->tx_mode < DC_TX_MODE_MIN || p->tx_mode > DC_TX_MODE_MAX)
        p->tx_mode = DC_TX_MODE_CW;
    if(p->dim < 0 || p->dim > 100)
        p->dim = 100;
    DC_Update();
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

    g.cfg.cw_carrier_freq = 500;
    g.cfg.ft8_carrier_freq = 500;
    g.cfg.cw_wpm = 40;
    g.cfg.tx_mode = DC_TX_MODE_CW;

    DC_Update();
}

DataConfig_t* DC_Get(void)
{
    return &g;
}
