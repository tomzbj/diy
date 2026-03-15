#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim6;

extern "C" void TIM6_DAC_IRQHandler(void)
{
  if(0) {
    static int count = 0;
    ++count %= 100000;
    if(!count)
      printf("TIM6\n");
  }
  HAL_TIM_IRQHandler(&htim6);
}

void TIMER6_Config(void)
{
  __HAL_RCC_TIM6_CLK_ENABLE();

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.Period = 1199;    // 120M / 1200 = 100k
//  htim6.Init.Period = 69;    // 84M / 1680 = 50k
  htim6.Init.ClockDivision = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.RepetitionCounter = 0;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  test_if(HAL_TIM_Base_Init(&htim6));

  TIM_MasterConfigTypeDef tmcis;
  tmcis.MasterOutputTrigger = TIM_TRGO_UPDATE;
  tmcis.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim6, &tmcis);
//  test_if(HAL_TIM_Base_Start_IT(&htim6));
  test_if(HAL_TIM_Base_Start(&htim6));
//  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 3, 0);
//  HAL_NVIC_EnableIRQ (TIM6_DAC_IRQn);

  print_log();
}
