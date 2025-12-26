#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim1;

extern "C" void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1);
}

void TIMER1_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_12;
  gis.Mode = GPIO_MODE_AF_PP;    //OUTPUT_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(GPIOA, &gis);

  __HAL_RCC_TIM1_CLK_ENABLE();
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&htim1) != HAL_OK)
    Error_Handler();

  TIM_ClockConfigTypeDef tccs;
  tccs.ClockFilter = 0;
  tccs.ClockPolarity = TIM_ETRPOLARITY_NONINVERTED;
  tccs.ClockPrescaler = TIM_ETRPRESCALER_DIV1;
  tccs.ClockSource = TIM_CLOCKSOURCE_ETRMODE1;
  HAL_TIM_ConfigClockSource(&htim1, &tccs);
  if(HAL_TIM_Base_Start_IT(&htim1) != HAL_OK)
    Error_Handler();

  HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ (TIM1_BRK_UP_TRG_COM_IRQn);

  /*  TIM_OC_InitTypeDef tois;
   tois.OCMode = TIM_OCMODE_PWM1;
   tois.OCPolarity = TIM_OCPOLARITY_HIGH;
   tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
   tois.OCFastMode = TIM_OCFAST_DISABLE;
   tois.OCIdleState = TIM_OCIDLESTATE_RESET;
   tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
   tois.Pulse = 40;
   if(HAL_TIM_PWM_ConfigChannel(&htim1, &tois, TIM_CHANNEL_2) != HAL_OK) Error_Handler();
   if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) != HAL_OK) Error_Handler(); */
  print_log();
}
