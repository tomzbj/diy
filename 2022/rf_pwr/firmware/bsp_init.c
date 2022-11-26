#include "misc.h"
#include "platform.h"

void BSP_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;

    SysTick_Config(rcu_clock_freq_get(CK_SYS) / 1000);
    FLASH_EEPROM_Config(0x08002000 - 1024, 1024);
    DC_Config();
    GPIO_Config();
    DISP_Config();
    ADC_Config();
    TASKS_Config();
//
    print_log();
}
