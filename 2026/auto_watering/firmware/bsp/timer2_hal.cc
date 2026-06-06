#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim2;

extern "C" void TIM2_IRQHandler(void)
{
  if(1) {
    static int count = 0;
    ++count %= 240000;
    if(!count)
      printf("TIM2\n");
  }
  HAL_TIM_IRQHandler(&htim2);
}

void TIMER2_Config(void)
{
  __HAL_RCC_TIM2_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_AFIO_REMAP_TIM2_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_15;
  gis.Mode = GPIO_MODE_AF_PP;    //OUTPUT_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
//  gis.Alternate = GPIO_AF2_TIM2;
  HAL_GPIO_Init(GPIOA, &gis);

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.Period = 999;    // 48M / 1000 = 48k
  htim2.Init.ClockDivision = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.RepetitionCounter = 0;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  test_if(HAL_TIM_Base_Init(&htim2));
  test_if(HAL_TIM_Base_Start(&htim2));

  TIM_OC_InitTypeDef tois = {0};
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = 0;
  if(HAL_TIM_PWM_ConfigChannel(&htim2, &tois, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();

//  TIM_MasterConfigTypeDef tmcis;
//  tmcis.MasterOutputTrigger = TIM_TRGO_UPDATE;
//  tmcis.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  HAL_TIMEx_MasterConfigSynchronization(&htim2, &tmcis);
//  test_if(HAL_TIM_Base_Start_IT(&htim2));
//  HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
//  HAL_NVIC_EnableIRQ (TIM2_IRQn);

  print_log();
}
