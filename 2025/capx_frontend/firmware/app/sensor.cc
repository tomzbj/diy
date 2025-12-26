#include "misc.h"
#include "platform.h"
#include "sensor.h"
#include "zi2c.h"
#include "lm75.h"

ZI2C i2c(PA5, PA6, 0x48, _delay_us, 3);
LM75 lm75(i2c);

void SENSOR_Config(void)
{
  // PA5 as SDA, PA6 as SCL
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_5;
  gis.Mode = GPIO_MODE_OUTPUT_OD;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Pin = GPIO_PIN_6;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &gis);

  HAL_Delay(100);
}
