#include "fan.h"
#include "misc.h"
#include "platform.h"

void FAN::init(void)
{
  GPIO_InitTypeDef gis;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  auto pins = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6;
  gis.Pin = pins;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  //    gis.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOB, &gis);
  off();
}

void FAN::update(void)
{
  TIMER_SetPWM(pwm_val_[state_]);
  ZPin::write(PB1, pwm_val_[state_]);
  GPIOA->ODR = (GPIOA->ODR & 0xb3) | led_[state_];
}
