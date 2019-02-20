#include "misc.h"

#define DO_COPY_VECTOR  0       // copy interrupt vectors to sram or not

//#define PLL_SOURCE_HSI            // HSI (~8MHz) -> PLL CLK -> SYS CLK
//#define PLL_SOURCE_HSE        // HSE (8MHz)  -> PLL CLK -> SYS CLK
//#define PLL_SOURCE_HSE_BYPASS // HSE bypassed <- EXT CLK (8M, from ST-LINK)
#define PLL_OFF             // if HSE works, use HSE; or use HSI

static void CopyVector(void)
{
    unsigned long* sram_base = (unsigned long*)0x20000000;
    unsigned long* app_base = (unsigned long*)0x08002000;
    for(int i = 0; i < 192; i += 4)
        *(sram_base + i) = *(app_base + i);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
    __enable_irq();
}

void SystemInit(void)
{
    RCC_DeInit();
    if(DO_COPY_VECTOR)
        CopyVector();

// SYSCLK, HCLK, PCLK configuration
#if defined (PLL_SOURCE_HSI) // At this stage the HSI is already enabled */
    FLASH_SetLatency(FLASH_Latency_1);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLKConfig(RCC_HCLK_Div1);
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
    RCC_PLLCmd(ENABLE);

    ( {  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);}); // Wait till PLL is ready
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    ( {  while(RCC_GetSYSCLKSource() != 0x08);}); // Wait till PLL is ready
#elif defined (PLL_SOURCE_HSE) || defined (PLL_SOURCE_HSE_BYPASS)
#if defined (PLL_SOURCE_HSE)
    RCC_HSEConfig(RCC_HSE_ON);
#elif defined (PLL_SOURCE_HSE_BYPASS)
    RCC_HSEConfig(RCC_HSE_Bypass);
#endif /* PLL_SOURCE_HSE */
    unsigned long HSEStatus = RCC_WaitForHSEStartUp();
    if(HSEStatus == 0x01) {
        FLASH_SetLatency(FLASH_Latency_1);
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLKConfig(RCC_HCLK_Div1);
        RCC_PREDIV1Config(RCC_PREDIV1_Div2);
        RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);
        RCC_PLLCmd(ENABLE);

        ( {  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);}); // Wait till PLL is ready
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        ( {  while(RCC_GetSYSCLKSource() != 0x08);}); // Wait till PLL is ready
    }
    else {
        // If HSE fails to start-up, do something here
    }
#else
    FLASH_SetLatency(FLASH_Latency_0);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLKConfig(RCC_HCLK_Div1);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    ( {  while(RCC_GetSYSCLKSource() != 0x00);});
#endif
}
