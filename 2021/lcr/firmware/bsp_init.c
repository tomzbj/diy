#include "misc.h"
#include "platform.h"

void zu_parser(const void* msg, int size, int source)
{
    CLI_Parse(msg, size, 0);
}

void BSP_Init(void)
{
    SystemInit();
    SysTick_Config(72000);
    GPIO_Config();
    static zu_cfg_t cfg;
    cfg.parser_f = zu_parser;
    cfg.readbyte_f = USART_ReadByte;
    cfg.writebyte_f = USART_WriteByte;
    zu_init(&cfg);
    USART_Config();
    FLASH_EEPROM_Config(0x08002000, 2048);
    DC_Config();
    TIMER_Config();
    DAC_Config();
    ADC_Config();

//    DataConfig_t* pdc = DC_Get();
//    ADC_Config(pdc->data.buf);

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    TASKS_Config();
}
