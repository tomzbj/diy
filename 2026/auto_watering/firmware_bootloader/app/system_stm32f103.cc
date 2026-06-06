#include "misc.h"

void* __dso_handle = 0;
uint32_t SystemCoreClock = 12000000UL;
const uint8_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};

extern "C" void SystemInit(void)
{
  __enable_irq();
  HAL_Init();
  HAL_RCC_DeInit();
//  return;
  RCC_ClkInitTypeDef rcis = {0};
  RCC_OscInitTypeDef rois = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  rois.HSIState = RCC_HSI_ON;
  rois.HSEState = RCC_HSE_OFF;
  rois.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//  rois.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  rois.PLL.PLLState = RCC_PLL_ON;
  rois.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  rois.PLL.PLLMUL = RCC_PLL_MUL16;    // 192M with 12M xtal
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    while(1);
  rcis.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
    | RCC_CLOCKTYPE_PCLK2);
  rcis.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  rcis.AHBCLKDivider = RCC_SYSCLK_DIV1;
  rcis.APB2CLKDivider = RCC_HCLK_DIV2;
  rcis.APB1CLKDivider = RCC_HCLK_DIV2;
  if(HAL_RCC_ClockConfig(&rcis, FLASH_LATENCY_2) != HAL_OK)
    while(1);
}
