#include "misc.h"
#include "platform.h"

//void* __dso_handle = 0;
const uint8_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};
uint32_t SystemCoreClock = 48000000UL;

extern "C" void SystemInit(void)
{
  __enable_irq();
  HAL_Init();
  HAL_RCC_DeInit();
  RCC_ClkInitTypeDef rcis = {0};
  RCC_OscInitTypeDef rocis = {0};
  rocis.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  rocis.HSIState = RCC_HSI_ON;
  rocis.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  rocis.PLL.PLLState = RCC_PLL_ON;
  rocis.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  rocis.PLL.PREDIV = RCC_PREDIV_DIV2;
  rocis.PLL.PLLMUL = RCC_PLL_MUL12;
  if(HAL_RCC_OscConfig(&rocis) != HAL_OK)
    while(1);
  rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB1CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_1) != HAL_OK)
    while(1); /* Initialization Error */
}
