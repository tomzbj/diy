#include "misc.h"
#include "platform.h"

void zu_parser(const void* msg, int size, int source)
{
    CLI_Parse(msg, size);
}

void BSP_Init(void)
{
    SystemInit();
    SysTick_Config(72000UL);
    FLASH_EEPROM_Config(0x08002800, 2048);
    GPIO_Config();

    static zu_cfg_t cfg;
    cfg.parser_f = zu_parser;
    cfg.readbyte_f = USART_ReadByte;
    cfg.writebyte_f = USART_WriteByte;
    zu_init(&cfg);
    USART_Config();

    DC_Config();
    SPI_Config();
    DISP_Config();
    TIMER_Config();
    ADC_Config();
    DAC_Config();
    KEY_Config();

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    TASKS_Config();
}
