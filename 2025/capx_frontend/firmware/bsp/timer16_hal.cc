#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim16;

extern "C" void TIM16_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim16);
}

void TIMER16_Config(void)
{
  __HAL_RCC_TIM16_CLK_ENABLE();
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 999;
  htim16.Init.Period = 2499;
  htim16.Init.ClockDivision = 0;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&htim16) != HAL_OK)
    Error_Handler();

  if(HAL_TIM_Base_Start_IT(&htim16) != HAL_OK)
    Error_Handler();

  HAL_NVIC_SetPriority(TIM16_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ (TIM16_IRQn);

  print_log();
}
