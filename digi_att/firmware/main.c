#include "misc.h"
#include "platform.h"

void parser(const void* msg, int size, int source)
{
}

int main(void)
{
    cli();
    {
        static zu_cfg_t cfg;
        cfg.parser_f = parser;
        cfg.readbyte_f = USART_ReadByte;
        cfg.writebyte_f = USART_WriteByte;
        zu_init(&cfg);
    }
    DC_Config();
    USART_Config();
    SSD_Config();
    KEY_Config();
    TIMER_Config();
    REL_Config();
    TASKS_Config();
    sei();

    while(1) {
        zt_poll();
    }
    return 0;
}
