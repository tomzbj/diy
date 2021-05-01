#include "misc.h"
#include "platform.h"

void SystemInit(void)
{
}

void zu_parser(const void* msg, int size, int source)
{
    CLI_Parse(msg, size);
}

int main(void)
{
    RCC_DeInit();
    RCC_HCLKConfig(RCC_SYSCLK_Div2);    // AHB CLK = SYSCLK
    SysTick_Config(4000UL);
    __enable_irq();
    GPIO_Config();
    zu_cfg_t cfg;
    cfg.parser_f = zu_parser;
    cfg.readbyte_f = USART_ReadByte;
    cfg.writebyte_f = USART_WriteByte;
    zu_init(&cfg);
    USART_Config();
    xdev_out(USART_WriteByte);
    TASKS_Config();
    SPI1_Config();
    RFID_Config();
    KEY_Config();
    DISP_Config();

    while(1) {
        zu_poll();
        zt_poll();
    }
    return 0;
}
