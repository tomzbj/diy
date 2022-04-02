#include "platform.h"

static void zu_parse(const void* msg, int size, int source)
{
    if(source == 0) {
        unsigned char c = *(unsigned char*)msg;
        if(c == '?' || c == '#')
            CLI_Parse(msg, size, 0);
        else if(c >= 0x90 && c <= 0x9e)
            fl_parse(msg, size);
    }
}

void BSP_Init(void)
{
    SystemInit();
    SysTick_Config(48000UL);
    FLASH_EEPROM_Config(0x08001c00, 1024);
    DC_Config();
    GPIO_Config();
    static zu_cfg_t cfg;
    cfg.parser_f = zu_parse;
    cfg.readbyte_f = USART_ReadByte;
    cfg.writebyte_f = USART_WriteByte;
    zu_init(&cfg);
    USART_Config();
    SPI_Config();
    TIMER_Config();
    LCD_Config();
    FLASH_Config();
    KEY_Config();
    EVENTS_Config();
    TASKS_Config();

    AUDIO_StartPlay("mtimein1_8.wav");

    printf("Initializing... done.\n");
}
