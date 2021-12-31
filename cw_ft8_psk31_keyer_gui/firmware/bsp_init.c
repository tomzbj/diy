#include "misc.h"
#include "platform.h"

static void parser_f(const void* pmsg, int size, int source)
{
    char* msg = (char*)pmsg;
    if(msg[0] == '#')
        CLI_Parse(msg, size, source);
    else if(msg[0] >= 0x90 && msg[0] <= 0x9f)
        fl_parse(msg, size);
    else if(msg[0] == '$') {
        msg[0] = ' ';
        RF_Parse(msg, size);
    }
}

void BSP_Init(void)
{
    SysTick_Config(108000UL);    //    SysTick_Config(0xfffffful);
    FLASH_EEPROM_Config(0x08002000 - 1024, 1024);
    DC_Config();
    GPIO_Config();
    {
        static zu_cfg_t cfg;
        cfg.parser_f = parser_f;
        cfg.readbyte_f = USART_ReadByte;
        cfg.writebyte_f = USART_WriteByte;
        zu_init(&cfg);
    }
    USART_Config();
    SPI_Config();
    DAC_Config();
    FLASH_Config();
    RF_Config(); // rf must be before timer
    TIMER_Config(); // timer must be before disp
    DISP_Config();
    RTC_Config();
    TASKS_Config();
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    printf("[%s: %d] ALL Initialized.\n", __FILE__, __LINE__);
}
