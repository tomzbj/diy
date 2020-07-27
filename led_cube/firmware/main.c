#include "misc.h"
#include <string.h>
#include "platform.h"
#include "tasks.h"

void SystemInit(void)    // called by startup script
{
    memcpy((void*)(0x20000000), (void*)0x08002800, 256);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
    __enable_irq();

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    FLASH_SetLatency(FLASH_Latency_1);
    if(RCC_WaitForHSEStartUp() == 0x01) {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
        RCC_PCLKConfig(RCC_HCLK_Div1);    // APB1 CLK = SYSCLK
        RCC_PREDIV1Config(RCC_PREDIV1_Div1);
        RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_3);    // 16M * 3 = 48M
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
    }
}

int main(void)
{
    void BSP_Init(void);
    FLASH_EEPROM_Config(0x08002800 - 2048, 2048);
    BSP_Init();
    TASKS_Config();
    while(1) {
        USART_Poll();
        TASKS_Poll();
    }
    return 0;
}
