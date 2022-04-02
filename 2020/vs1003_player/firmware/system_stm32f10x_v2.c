#include "stm32f10x.h"

#if defined (STM32F10X_LD_VL) || (defined STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
#define SYSCLK_FREQ_24MHz  24000000
#else
#define SYSCLK_FREQ_72MHz  72000000
#endif

#define VECT_TAB_OFFSET  0x0 // preserve 4096 bytes for bootloader

#ifdef SYSCLK_FREQ_HSE
uint32_t SystemCoreClock = SYSCLK_FREQ_HSE;
#elif defined SYSCLK_FREQ_24MHz
uint32_t SystemCoreClock = SYSCLK_FREQ_24MHz;
#elif defined SYSCLK_FREQ_72MHz
uint32_t SystemCoreClock = SYSCLK_FREQ_72MHz;
#else 
uint32_t SystemCoreClock = HSI_VALUE;
#endif

static void SetSysClock(void);

#ifdef SYSCLK_FREQ_HSE
static void SetSysClockToHSE(void);
#elif defined SYSCLK_FREQ_24MHz
static void SetSysClockTo24(void);
#elif defined SYSCLK_FREQ_72MHz
static void SetSysClockTo72(void);
#endif

void SystemInit(void)
{
//    RCC_DeInit();
    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, VECT_TAB_OFFSET);
    SetSysClock();
    __enable_irq(); 
}

void SystemCoreClockUpdate(void)
{
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    SystemCoreClock = clocks.SYSCLK_Frequency;
}

static void SetSysClock(void)
{
#ifdef SYSCLK_FREQ_HSE
    SetSysClockToHSE();
#elif defined SYSCLK_FREQ_24MHz
    SetSysClockTo24();
#elif defined SYSCLK_FREQ_72MHz
    SetSysClockTo72();
#endif 
} // If none of the define above is enabled, the HSI is used as System clock source (default after reset)

#ifdef SYSCLK_FREQ_HSE
static void SetSysClockToHSE(void)
{
    volatile unsigned long HSEStatus = 0;

    RCC_HSEConfig(RCC_HSE_ON);
    HSEStatus = RCC_WaitForHSEStartUp();

    if(HSEStatus == 0x01) {

#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_0);
#endif 
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);// APB2 CLK = SYSCLK
        RCC_PCLK1Config(RCC_HCLK_Div1);// APB1 CLK = SYSCLK
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

        while(RCC_GetSYSCLKSource() != 0x04);  /* Wait till HSE is used as system clock source */
    }
    else {
        // If HSE start-up failed, do something here.
    }
}
#elif defined SYSCLK_FREQ_24MHz

static void SetSysClockTo24(void)
{
    volatile unsigned long HSEStatus = 0;

    RCC_HSEConfig(RCC_HSE_ON);
    HSEStatus = RCC_WaitForHSEStartUp();

    if(HSEStatus == 0x01) {

#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_0);
#endif
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);     // APB2 CLK = SYSCLK
        RCC_PCLK1Config(RCC_HCLK_Div1);     // APB1 CLK = SYSCLK

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
        RCC_PREDIV1Config(RCC_PREDIV1_Source_HSE, RCC_PREDIV1_Div2);
        RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);
#else
        RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_6);
#endif
        RCC_PLLCmd(ENABLE);

        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); /* Wait till PLL is ready */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); /* Wait till PLL is ready */
        while(RCC_GetSYSCLKSource() != 0x08);  /* Wait till PLL is used as system clock source */
    }
    else {
    }
}

#elif defined SYSCLK_FREQ_72MHz
static void SetSysClockTo72(void)
{
    volatile unsigned long HSEStatus = 0;

    RCC_HSEConfig(RCC_HSE_ON);
    HSEStatus = RCC_WaitForHSEStartUp();

    if(HSEStatus == 0x01) {

#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);
#endif
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB CLK = SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);     // APB2 CLK = SYSCLK
        RCC_PCLK1Config(RCC_HCLK_Div2);     // APB1 CLK = SYSCLK

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
        RCC_PREDIV1Config(RCC_PREDIV1_Source_HSE, RCC_PREDIV1_Div1);
        RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#else
        RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
#endif
        RCC_PLLCmd(ENABLE);

        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); /* Wait till PLL is ready */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); /* Wait till PLL is ready */
        while(RCC_GetSYSCLKSource() != 0x08);  /* Wait till PLL is used as system clock source */
    }
    else {
    }
}
#endif
