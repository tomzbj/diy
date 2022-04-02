#include <stdio.h>
#include "usart_f1.h"
#include "adc_f1.h"
#include "stm32f10x.h"
#include "timer.h"
#include "batt.h"
#include "dac_f1.h"

void SystemInit(void)   // called by startup script
{
    RCC_DeInit();
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0); // do not use bootloader
    RCC_HSEConfig(RCC_HSE_ON);
    if(RCC_WaitForHSEStartUp() == 0x01) {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);    // APB2 CLK = SYSCLK
        RCC_PCLK1Config(RCC_HCLK_Div1);    // APB1 CLK = SYSCLK
        RCC_PREDIV1Config(RCC_PREDIV1_Source_HSE, RCC_PREDIV1_Div2);
        RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_3); // 16M / 2 * 3 = 24M
        RCC_PLLCmd(ENABLE);
        ( {  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);});
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        ( {  while(RCC_GetSYSCLKSource() != 0x08);});
    }
}


static void Init(void)
{
    SysTick_Config(24000UL);
    USART_Config();
    ADC_Config();
    DAC_Config();
}

int main(void)
{
    Init();
    while(1) {
        USART_Poll();
    }
    return 0;
}
