#include "misc.h"
#include "platform.h"

void SystemInit(void)
{
    RCC_DeInit();
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);    // APB2 CLK = SYSCLK
    RCC_PCLK1Config(RCC_HCLK_Div1);    // APB1 CLK = SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    while(RCC_GetSYSCLKSource() != 0x00);
    __enable_irq();
}

int main(void)
{
    SysTick_Config(8000UL);
    FLASH_EEPROM_Config(0x08001c00, 1024);

    DC_Config();
    GPIO_Config();
    USART_Config();
    ADC_Config();
    TIMER_Config();
    TASKS_Config();

    while(1) {
        USART_Poll();
        TASKS_Poll();
    }
    return 0;
}
