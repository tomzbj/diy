#include "platform.h"

void bpsk_tx_start(void)
{
}

void bpsk_tx_char(int c)
{
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.cli_echo_state
        && pdc->cfg.cli_echo_mode == DC_CLI_ECHO_MODE_ASCII) {
        printf("%c", c);
        fflush(stdout);
    }
}

void bpsk_tx_end(void)
{
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.cli_echo_state) {
        printf("bpsk tx end.\n");
        fflush(stdout);
    }
}

void bpsk_tx_symbol(unsigned short data)
{
    DAC_SetChannel2Data(DAC_Align_12b_L, data);
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.spk_echo_state) {
        DAC_SetChannel1Data(DAC_Align_12b_L, data);
    }
}
