#include "misc.h"
#include "platform.h"

void SystemInit(void)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    if(RCC_WaitForHSEStartUp() == SUCCESS) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
        PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div1);
        RCC_PLLConfig(RCC_PLLSource_HSE, 16, 336, 4, 7);    // M, N, P, Q
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        FLASH_PrefetchBufferCmd(ENABLE);
        FLASH_InstructionCacheCmd(ENABLE);
        FLASH_DataCacheCmd(ENABLE);
        FLASH_SetLatency(FLASH_Latency_2);
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
    }
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000);
    __enable_irq();
}

int main(void)
{
    void BSP_Init(void);
    BSP_Init();
    while(1) {
        USART_Poll();
        TASKS_Poll();
        zplay_poll();
    }
}
