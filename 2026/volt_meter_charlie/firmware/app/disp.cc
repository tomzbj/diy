#include "misc.h"
#include "platform.h"
#include "zcharlie.h"

static const void set_dir(int pin_num, int dir)
{
  if(pin_num < 0)
    return;
  GPIO_TypeDef* ports[] = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA};
  const uint16_t pins[] = {GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1,
    GPIO_PIN_0};
  const auto& port = ports[pin_num];
  const auto& pin = pins[pin_num];
  if(dir == ZCHARLIE::IN) {
    GPIO_InitTypeDef gis = {0};
    gis.Pin = pin;
    gis.Mode = GPIO_MODE_INPUT;
    gis.Pull = GPIO_NOPULL;
    gis.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(port, &gis);
  }
  else {
    GPIO_InitTypeDef gis = {0};
    gis.Pin = pin;
    gis.Mode = GPIO_MODE_OUTPUT_PP;
    gis.Pull = GPIO_NOPULL;
    gis.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(port, &gis);

    if(dir == ZCHARLIE::L)
      port->BRR = pin;
    else if(dir == ZCHARLIE::H)
      port->BSRR = pin;
  }
}

const ZCHARLIE::leds_t leds[24] = {
  //
  {1, 6}, {3, 6}, {5, 6}, {6, 4}, {4, 6}, {6, 5}, {1, 5}, {-1, -1},    //
  {5, 4}, {3, 5}, {4, 5}, {3, 4}, {6, 3}, {4, 3}, {5, 3}, {3, 1},    //
  {2, 3}, {2, 4}, {5, 2}, {2, 6}, {2, 5}, {3, 2}, {4, 2}, {2, 1}    //
};

ZCHARLIE charlie(set_dir, leds, 24, 6);

void DISP_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
//  charlie.write((uint8_t*)"\xff\xff\xff", 3);
//  charlie.write(321, 1);
  charlie.write(1500.0f);
}

void DISP_Poll(void)
{
  charlie.poll_pin();
//  charlie.poll_led();
}

