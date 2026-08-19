#include "misc.h"
#include "platform.h"

static TIM_HandleTypeDef htim5;

// TIM5 on APB1: APB1=/2 → timer clk = 48MHz (x2)
// Keep ~676Hz (previous 144M tune: 144M/16/13333); 48M/16/4440 ≈ 675.7Hz
static constexpr uint32_t TIM5_PSC = 15;
static constexpr uint32_t TIM5_ARR = 4439;

extern "C" void TIM5_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim5);
}

void TIMER5_SetDuty(int duty_permille)
{
  duty_permille = saturate(duty_permille, 0, 999);
  uint32_t pulse = (uint32_t)duty_permille * (TIM5_ARR + 1) / 1000;
  if(pulse > TIM5_ARR)
    pulse = TIM5_ARR;
  TIM5->CCR2 = pulse;
}

void TIMER5_Config(void)
{
  __HAL_RCC_TIM5_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();

  // PA1 = TIM5_CH2
  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_1;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = TIM5_PSC;
  htim5.Init.Period = TIM5_ARR;
  htim5.Init.ClockDivision = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.RepetitionCounter = 0;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  test_if(HAL_TIM_Base_Init(&htim5));
  test_if(HAL_TIM_Base_Start(&htim5));

  TIM_OC_InitTypeDef tois = {0};
  tois.OCMode = TIM_OCMODE_PWM1;
  tois.OCPolarity = TIM_OCPOLARITY_HIGH;
  tois.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  tois.OCFastMode = TIM_OCFAST_DISABLE;
  tois.OCIdleState = TIM_OCIDLESTATE_RESET;
  tois.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  tois.Pulse = 0;    // silent until TIMER5_SetDuty()
  if(HAL_TIM_PWM_ConfigChannel(&htim5, &tois, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();
  if(HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2) != HAL_OK)
    Error_Handler();

  print_log();
}
