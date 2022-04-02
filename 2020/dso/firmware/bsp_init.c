#include "misc.h"
#include "platform.h"

void hc595_setcs(int state)
{
    (state) ? (GPIOA->BSRR = GPIO_Pin_6) : (GPIOA->BRR = GPIO_Pin_6);
}

void BSP_Init(void)
{
    SystemInit();
    SysTick_Config(80000);
    GPIO_Config();
    USART_Config();
    FLASH_EEPROM_Config(0x08002000, 2048);
    DC_Config();
    SPI_Config();

    {
        static hc595_cfg_t cfg;
        cfg.cs_f = hc595_setcs;
        cfg.delay_f = _delay_ms;
        cfg.writebyte_f = SPI1_WriteByte;
        HC595_Config(&cfg);
    }
    DISP_Config();
    TIMER_Config();
//    DAC_Config();

    DataConfig_t* pdc = DC_Get();
    ADC_Config(pdc->data.buf);
//    OPAMP_Config();
//    EXTI_Config();
//    unsigned short page_size = 2048;

    KEY_Config();
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    TASKS_Config();
}
