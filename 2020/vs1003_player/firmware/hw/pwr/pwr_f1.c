#include <stdbool.h>
#include "misc.h"
#include "pwr_f1.h"
#include "spiflash.h"
#include "ff.h"

extern bool g_status_sleep;

void PWR_Sleep(void)
{
    NVIC_DisableIRQ(EXTI2_IRQn);
//    SPIFLASH_EnterLowPowerMode();
    VS1003_PowerDown();
    EXTI_ClearITPendingBit(EXTI_Line2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    NVIC_EnableIRQ(EXTI2_IRQn);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    g_status_sleep = true;
    SystemInit();
    NVIC_DisableIRQ(EXTI2_IRQn);
    _delay_ms_loop(100);
    VS1003_Config();
    _delay_ms_loop(100);
//    SPIFLASH_LeaveLowPowerMode();
    _delay_ms_loop(100);
    NVIC_EnableIRQ(EXTI2_IRQn);
    g_status_sleep = false;
//    NVIC_SystemReset();
}
