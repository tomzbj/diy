#include "misc.h"
#include "platform.h"

void* __dso_handle = 0;
const uint8_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};
uint32_t SystemCoreClock = 48000000UL;

extern "C" void SystemInit(void)
{
    RCC_ClkInitTypeDef rcis;
    RCC_OscInitTypeDef rocis;
    rocis.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    rocis.HSEState = RCC_HSE_ON;
    rocis.PLL.PLLState = RCC_PLL_OFF;       // 不使用PLL, 节约电池
    rocis.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    rocis.PLL.PREDIV = RCC_PREDIV_DIV1;
    rocis.PLL.PLLMUL = RCC_PLL_MUL3;
    if(HAL_RCC_OscConfig(&rocis) != HAL_OK)
        while(1);

    rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
//    rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    rcis.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;       // 以16M HSE为主时钟
    rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
    rcis.APB1CLKDivider = RCC_HCLK_DIV1;
    if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_1) != HAL_OK)
        while(1); /* Initialization Error */
}

int main(void)
{
    BSP_Init();
    TASKS_Config();
    while(1);    // Never reach here
}
