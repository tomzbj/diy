#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim1;

void TIMER_SetPWM(int pwm)
{
  printf("### %d\n", pwm);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwm);
  if(pwm == 0)
    __HAL_TIM_DISABLE(&htim1);
  else
    __HAL_TIM_ENABLE(&htim1);
}

void TIMER_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_1;
  gis.Mode = GPIO_MODE_AF_PP;    //OUTPUT_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF13_TIM1;
  HAL_GPIO_Init(GPIOA, &gis);

  __HAL_RCC_TIM1_CLK_ENABLE();
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&htim1) != HAL_OK)
    Error_Handler();

  TIM_OC_InitTypeDef tois;
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = 500;
  if(HAL_TIM_PWM_ConfigChannel(&htim1, &tois, TIM_CHANNEL_4) != HAL_OK) {
    Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4) != HAL_OK) {
    Error_Handler();
  }
  _dbg();
}
