#include "misc.h"
#include "platform.h"
#include <cmath>

static TIM_HandleTypeDef htim3;

extern "C" void TIM3_IRQHandler(void)
{
  if(0) {
    static int count = 0;
    ++count %= 24000;
    if(!count)
      printf("#\n");
  }
  HAL_TIM_IRQHandler(&htim3);
}

void TIMER3_SetCurr(float curr)
{
  curr = saturate(curr, 0.0f, 998.0f);
  TIM3->CCR2 = int(curr * 0.1f * 11.0f / 3300.0f * 990.0f);
}

void TIMER3_SetDuty(int duty)
{
  TIM3->CCR2 = duty;
}

void TIMER3_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM3_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_5;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF13_TIM3;
  HAL_GPIO_Init(GPIOA, &gis);

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.Period = 999;    // 48e6/1000=48k
  htim3.Init.ClockDivision = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.RepetitionCounter = 0;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if(HAL_TIM_Base_Init(&htim3) != HAL_OK)
    Error_Handler();

  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ (TIM3_IRQn);

  TIM_OC_InitTypeDef tois = {0};
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = 30;
  if(HAL_TIM_PWM_ConfigChannel(&htim3, &tois, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

  print_log();
}
