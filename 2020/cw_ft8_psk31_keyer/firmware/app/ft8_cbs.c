#include "platform.h"

static int count;

void ft8_tx_start(void)
{
    count = 0;
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.spk_echo_state)
        TIM_Cmd(TIM6, ENABLE);
}

void ft8_tx_end(void)
{
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.cli_echo_state) {
        printf("\n");
        fflush(stdout);
    }
    TIM_Cmd(TIM6, DISABLE);
}

void ft8_tx_symbol(int c)
{
    printf("%d:%d\n", count, c);
    count++;
    float freq = 200 + c * 6.25;
    TIM_SetAutoreload(TIM6, (int)(48000000UL / 256.0 / freq - 1));
}
