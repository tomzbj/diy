#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim3;

extern "C" void TIM3_IRQHandler(void)
{
  if(0) {
    static int count = 0;
    ++count %= 8000;
    if(!count)
      printf("TIM3\n");
  }
  HAL_TIM_IRQHandler(&htim3);
}

void TIMER3_Config(void)
{
  __HAL_RCC_TIM3_CLK_ENABLE();

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.Period = 14999;    // 130M / 1300 = 100k
  htim3.Init.ClockDivision = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.RepetitionCounter = 0;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  test_if(HAL_TIM_Base_Init(&htim3));

  TIM_MasterConfigTypeDef tmcis;
  tmcis.MasterOutputTrigger = TIM_TRGO_UPDATE;
  tmcis.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &tmcis);
  test_if(HAL_TIM_Base_Start_IT(&htim3));
//  test_if(HAL_TIM_Base_Start(&htim3));
  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ (TIM3_IRQn);

  print_log();
}
