#include "misc.h"

static IWDG_HandleTypeDef hiwdg = {0};

void IWDG_Feed(void)
{
  if(HAL_IWDG_Refresh(&hiwdg) != HAL_OK)
    Error_Handler();
}

void IWDG_Config(void)
{
  RCC_OscInitTypeDef rois = {0};
  rois.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  rois.LSIState = RCC_LSI_ON;
  rois.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&rois) != HAL_OK)
    Error_Handler();

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
  hiwdg.Init.Reload = 40000 / 128;
  if(HAL_IWDG_Init(&hiwdg) != HAL_OK)
    Error_Handler();
}
