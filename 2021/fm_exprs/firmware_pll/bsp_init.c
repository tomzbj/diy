#include "misc.h"
#include "platform.h"

static void zu_parser(const void* msg, int size, int source)
{
    CLI_Parse(msg, size);
}

void BSP_Init(void)
{
    SysTick_Config(48000UL);
    DC_Config();
    GPIO_Config();
    zu_cfg_t cfg;
    cfg.parser_f = zu_parser;
    cfg.readbyte_f = USART_ReadByte;
    cfg.writebyte_f = USART_WriteByte;
    zu_init(&cfg);
    USART_Config();
    xdev_out(USART_WriteByte);
    TASKS_Config();
    KEY_Config();
    DISP_Config();
    VFO_Config();
    TIMER_Config();
    TIM_SetCompare1(TIM14, 600);
    ADC_Config();
}
