#include "misc.h"
#include "platform.h"
#include <cmath>

static TIM_HandleTypeDef htim15;

extern "C" void TIM15_IRQHandler(void)
{
  if(0) {
    static int count = 0;
    ++count %= 24000;
    if(!count)
      printf("#\n");
  }
  HAL_TIM_IRQHandler(&htim15);
}

static float saturate(float val, float min, float max)
{
  if(val > max)
    val = max;
  if(val < min)
    val = min;
  return val;
}
void TIMER15_SetVoltCurr(float volt, float curr)
{
  volt = saturate(volt, 1.0f, 10.0f);
  curr = saturate(curr, 0.0f, 350.0f);
  TIM15->CCR2 = int(volt / (27.0f / 10.0f + 1) / 3.3f * 990.0f);
  TIM15->CCR1 = int(curr * 0.33f * 20.0f / 3.3f);
}

void TIMER15_SetDuty(int channel, int duty)
{
  if(channel == 1)
    TIM15->CCR1 = duty;
  else if(channel == 2)
    TIM15->CCR2 = duty;
}

void TIMER15_Config(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_TIM15_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF1_TIM15;
  HAL_GPIO_Init(GPIOB, &gis);

  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 0;
  htim15.Init.Period = 999;    // 48e6/1000=48k
  htim15.Init.ClockDivision = 0;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if(HAL_TIM_Base_Init(&htim15) != HAL_OK)
    Error_Handler();

  HAL_NVIC_SetPriority(TIM15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM15_IRQn);

  TIM_OC_InitTypeDef tois = {0};
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = 250;
  if(HAL_TIM_PWM_ConfigChannel(&htim15, &tois, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  tois.Pulse = 150;
  if(HAL_TIM_PWM_ConfigChannel(&htim15, &tois, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();
  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);

  print_log();
}
