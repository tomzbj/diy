#include "misc.h"
#include "platform.h"

uint32_t SystemCoreClock = 24000000UL;
const uint32_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint32_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};

void SystemInit(void)    // called by startup script
{
  HAL_Init();
  HAL_RCC_DeInit();
  RCC_OscInitTypeDef rois = {0};
  RCC_ClkInitTypeDef rcis = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI
    | RCC_OSCILLATORTYPE_LSI;

  rois.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  rois.HSIState = RCC_HSI_ON;
  rois.HSIDiv = RCC_HSI_DIV1;
  rois.HSICalibrationValue = RCC_HSICALIBRATION_24MHz;
  rois.HSEState = RCC_HSE_OFF;
  rois.HSEFreq = RCC_HSE_8_16MHz;
  rois.LSIState = RCC_LSI_OFF;

  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    Error_Handler();
  rcis.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB1CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_1) != HAL_OK)
    Error_Handler();
  __enable_irq();
}

