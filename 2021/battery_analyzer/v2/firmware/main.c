#include "misc.h"
#include "platform.h"

void SystemInit(void)
{
    memcpy((void*)(0x20000000), (void*)(0x08002000), 256);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
    __enable_irq();

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    FLASH_SetLatency(FLASH_Latency_1);

    if(RCC_WaitForHSEStartUp() == 0x01) {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
        RCC_PCLKConfig(RCC_HCLK_Div1);    // APB1 CLK = SYSCLK
        RCC_PLLConfig(RCC_PLLSource_HSE, RCC_PLLMul_3);    // 8M * 6 = 48M
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
    }
}

int main(void)
{
    void BSP_Init(void);
    BSP_Init();
    while(1) {
        zu_poll();
        TASKS_Poll();
    }
    return 0;
}
