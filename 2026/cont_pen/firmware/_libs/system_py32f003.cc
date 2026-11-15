#include "misc.h"
#include "platform.h"

uint32_t SystemCoreClock = 8000000UL;
const uint32_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint32_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};
const uint32_t HSIFreqTable[8] = {4000000U, 8000000U, 16000000U, 22120000U, 24000000U,
  4000000U, 4000000U, 4000000U};

void SystemInit(void)    // called by startup script
{
  HAL_Init();
  RCC_OscInitTypeDef rois = {0};
  RCC_ClkInitTypeDef rcis = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  rois.HSIState = RCC_HSI_ON;
  rois.HSIDiv = RCC_HSI_DIV1;
  rois.HSICalibrationValue = RCC_HSICALIBRATION_4MHz;
  rois.HSEState = RCC_HSE_OFF;
  rois.LSIState = RCC_LSI_ON;

  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    Error_Handler();

  rcis.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
//  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_LSI;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB1CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_0) != HAL_OK)
    Error_Handler();
}

