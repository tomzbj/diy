#include "misc.h"
#include "platform.h"

void zu_parser(const void* msg, int size, int source)
{
    CLI_Parse(msg, size);
}

unsigned char zu_readbyte(int source)
{
    return USART_ReadByte();
}

void zu_writebyte(unsigned char data, int source)
{
    USART_WriteByte(data);
}

void BSP_Init(void)
{
    SystemInit();
    FLASH_EEPROM_Config(0x08002000 - 1024, 1024);
    DC_Config();

    SysTick_Config(48000UL);

    GPIO_Config();

    static zu_cfg_t cfg;
    cfg.parser_f = zu_parser;
    cfg.readbyte_f = zu_readbyte;
    cfg.writebyte_f = zu_writebyte;
    zu_init(&cfg);

    USART_Config();

    ADC_Config();
    DAC_Config();
    MENU_Config();
    DISP_Config();
    KEY_Config();
    TASKS_Config();

    xprintf("init done.\n");
}
