#include "platform.h"

void cw_tx_start(void)
{
}

void cw_tx_char(int c)
{
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.cli_echo_state
        && pdc->cfg.cli_echo_mode == DC_CLI_ECHO_MODE_ASCII) {
        printf("%c", c);
        fflush(stdout);
    }
}

void cw_tx_end(void)
{
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.cli_echo_state) {
        printf("\n");
        fflush(stdout);
    }
}

void cw_tx_symbol(int c)
{
    DataConfig_t* pdc = DC_Get();
    static int oldc = 1;

    // echo
    if(pdc->cfg.cli_echo_state
        && pdc->cfg.cli_echo_mode == DC_CLI_ECHO_MODE_CW) {
        static int c1, c0;
        if(oldc != c) {
            if(c == 0) {
                if(c1 == 1)
                    printf(".");
                else if(c1 == 3)
                    printf("-");
            }
            else {
                if(c0 == 3)
                    printf(" ");
                if(c0 == 7) {
                    printf(" ");
                    printf(" ");
                }
            }
            c0 = 0;
            c1 = 0;
        }
        if(c == 0)
            c0++;
        else
            c1++;
        fflush(stdout);
    }
    oldc = c;
    if(c) {
//        GPIOC->ODR |= _BV(5);
        GPIOB->BSRR = GPIO_Pin_1;
        if(pdc->cfg.spk_echo_state)
            TIM_Cmd(TIM6, ENABLE);

    }
    else {
//        GPIOC->ODR &= ~_BV(5);
        TIM_Cmd(TIM6, DISABLE);
        GPIOB->BRR = GPIO_Pin_1;
    }
}
