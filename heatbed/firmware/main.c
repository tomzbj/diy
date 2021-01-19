#include "misc.h"
#include "platform.h"

void SystemInit(void)
{
    RCC_DeInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
    memcpy((void*)(0x20000000), (void*)(0x08002000), 256);
    FLASH_SetLatency(FLASH_Latency_1);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
    RCC_PCLKConfig(RCC_HCLK_Div1);    // APB1 CLK = SYSCLK
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);    // 4M * 12 = 48M
    RCC_PLLCmd(ENABLE);
    ( {  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);});
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    ( {  while(RCC_GetSYSCLKSource() != 0x08);});
    __enable_irq();
}

int main(void)
{
    void BSP_Init(void);
    BSP_Init();
    while(1) {
        USART_Poll();
        TASKS_Poll();
    }
    return 0;
}
