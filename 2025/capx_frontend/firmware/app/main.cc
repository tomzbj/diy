#include "misc.h"
#include "platform.h"

uint32_t SystemCoreClock = 25000000UL;
const uint32_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint32_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};
const uint32_t HSIFreqTable[8] = {4000000U, 8000000U, 16000000U, 22120000U, 24000000U,
  4000000U, 4000000U, 4000000U};

void SystemInit(void)    // called by startup script
{
  HAL_Init();
  RCC_OscInitTypeDef rois = {0};
  RCC_ClkInitTypeDef rcis = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  rois.HSEState = RCC_HSE_ON;
  rois.HSEFreq = RCC_HSE_16_32MHz;
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    Error_Handler();
  rcis.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB1CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_0) != HAL_OK)
    Error_Handler();
}

int main(void)
{
  void BSP_Init(void);
  BSP_Init();
  while(1)
    zt.poll();
  return 0;
}
