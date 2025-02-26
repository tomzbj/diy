#include "misc.h"
#include "platform.h"
#include "key.h"
#include "zkey.h"

static int key_read(int key_id);
static void key_action(int key_id, int action);

static ZKey::cbs_t cbs = {key_read, key_action};

int key_count[1], key_state[1];
ZKey zk(cbs, 1, 300, key_count, key_state);

static int key_read(int key_id)
{
  const zpin_t keys[1] = {PA9};
  return (!ZPin::read(keys[key_id]));
}

static void key_action(int key_id, int action)
{
  extern class ZEVENT ze;
  switch(action) {
    case ZKey::PRESS:
      ze.post(EVENTS::KEY_PRESSED);
      break;
    case ZKey::LONGPRESS:
      ze.post(EVENTS::KEY_LONGPRESSED);
      break;
  }
}

void KEY_Config(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  auto pins = GPIO_PIN_9;
  GPIO_InitTypeDef gis;

  gis.Pin = pins;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  //    gis.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &gis);

  _dbg();
}
