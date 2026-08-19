#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim3;

extern "C" void TIM3_IRQHandler(void)
{
  if(1) {
    static int count = 0;
    ++count %= 240000;
    if(!count)
      printf("TIM3\n");
  }
  HAL_TIM_IRQHandler(&htim3);
}

void TIMER3_Config(void)
{
  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_0;
  gis.Mode = GPIO_MODE_AF_PP;    //OUTPUT_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);

  htim3.Instance = TIM3;
  // TIMCLK=48MHz (APB1/2 with x2): 48M/48/1000 = 1kHz
  htim3.Init.Prescaler = 47;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.RepetitionCounter = 0;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  test_if(HAL_TIM_Base_Init(&htim3));
  test_if(HAL_TIM_Base_Start(&htim3));

  TIM_OC_InitTypeDef tois = {0};
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = 100;    // default backlight ~10%
  if(HAL_TIM_PWM_ConfigChannel(&htim3, &tois, TIM_CHANNEL_3) != HAL_OK)
    Error_Handler();
  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK)
    Error_Handler();

//  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
//  HAL_NVIC_EnableIRQ (TIM3_IRQn);

  print_log();
}
