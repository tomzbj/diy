#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim14;

void TIMER14_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_4;
  gis.Mode = GPIO_MODE_AF_PP;    //OUTPUT_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF4_TIM14;
  HAL_GPIO_Init(GPIOA, &gis);

  __HAL_RCC_TIM14_CLK_ENABLE();
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 0;
  htim14.Init.Period = 999;
  htim14.Init.ClockDivision = 0;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.RepetitionCounter = 0;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&htim14) != HAL_OK)
    Error_Handler();

  TIM_OC_InitTypeDef tois;
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = 500;
  if(HAL_TIM_PWM_ConfigChannel(&htim14, &tois, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  if(HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  _dbg();
}
