#include "lptim_hal.h"

static LPTIM_HandleTypeDef hlptim;

extern "C" void LPTIM1_IRQHandler(void)
{
  HAL_LPTIM_IRQHandler(&hlptim);
}

void LPTIM_Config(void)
{
  __HAL_RCC_LPTIM_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  RCC_PeriphCLKInitTypeDef pclkis;

  pclkis.PeriphClockSelection = RCC_PERIPHCLK_LPTIM;
  pclkis.LptimClockSelection = RCC_LPTIMCLKSOURCE_LSI;
  if(HAL_RCCEx_PeriphCLKConfig(&pclkis) != HAL_OK) {
    Error_Handler();
  }
  {
    hlptim.Instance = LPTIM;
    hlptim.Init.Prescaler = LPTIM_PRESCALER_DIV128;
    hlptim.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
    if(HAL_LPTIM_Init(&hlptim) != HAL_OK) {
      Error_Handler();
    }
  }

  HAL_NVIC_SetPriority(LPTIM1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ (LPTIM1_IRQn);
}
