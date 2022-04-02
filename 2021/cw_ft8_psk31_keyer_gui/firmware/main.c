#include "misc.h"
#include "platform.h"

void SystemInit(void)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    if(RCC_WaitForHSEStartUp() == 0x01) {
        FLASH_PrefetchBufferCmd(ENABLE);
        FLASH_SetLatency(FLASH_Latency_2);
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);     // APB2 CLK = SYSCLK
        RCC_PCLK1Config(RCC_HCLK_Div1);     // APB1 CLK = SYSCLK
        RCC_PREDIV1Config(RCC_PREDIV1_Div2);
        RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
    }
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);
    __enable_irq();
}

int main(void)
{
    void BSP_Init(void);
    BSP_Init();
    while(1) {
        zu_poll();
        zt_poll();
    }
}
