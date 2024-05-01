#include "misc.h"
#include "platform.h"

ZI2C i2c(PB10, PB11, 0x38, _delay_us, 10);
WHT20 sensor(i2c);

void SHT_Config(void)
{
    // PB10&11 as i2c for AHT/WHT20
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gis;
    gis.Pin = GPIO_PIN_10;
    gis.Mode = GPIO_MODE_OUTPUT_OD;
    gis.Pull = GPIO_PULLUP;
    gis.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gis);
    gis.Pin = GPIO_PIN_11;
    gis.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gis);

    _delay_ms(100);
    sensor.init();
}
