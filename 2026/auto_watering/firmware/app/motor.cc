#include "misc.h"
#include "motor.h"
#include "zpin.h"

void MOTOR_Config(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // PB10&11 as key
  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_15;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);
  print_log();
}

void MOTOR_Set(int state)
{
  if(state == MOTOR_RUN)
    ZPin::set(PB15);
  else
    ZPin::clear(PB15);
}

