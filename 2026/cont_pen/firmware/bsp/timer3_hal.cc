#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim3;

extern "C" void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim3);
  if(0) {
    static int count = 0;
    ++count %= 300;
    if(count == 0)
      printf("#\n");
  }
}

void TIMER3_Config(void)
{

  __HAL_RCC_TIM3_CLK_ENABLE();
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 2;
  htim3.Init.Period = 999;    // 4M / 10 / 1000 = 400Hz
  htim3.Init.ClockDivision = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.RepetitionCounter = 0;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&htim3) != HAL_OK)
    Error_Handler();

  TIM_OC_InitTypeDef tois = {0};
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = 850;
  if(HAL_TIM_PWM_ConfigChannel(&htim3, &tois, TIM_CHANNEL_1) != HAL_OK)
    Error_Handler();
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_LOW;
  tois.OCNPolarity = TIM_OCNPOLARITY_LOW;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_SET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_SET;

  if(HAL_TIM_PWM_ConfigChannel(&htim3, &tois, TIM_CHANNEL_3) != HAL_OK)
    Error_Handler();

//  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
//  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK) Error_Handler();
}

void TIMER3_BUZZ_Start(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_2;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  gis.Alternate = GPIO_AF13_TIM3;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Pin = GPIO_PIN_0;
//  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Alternate = GPIO_AF1_TIM3;
  HAL_GPIO_Init(GPIOB, &gis);

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void TIMER3_BUZZ_Stop(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_2;
  gis.Mode = GPIO_MODE_ANALOG;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOB, &gis);

  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
}
