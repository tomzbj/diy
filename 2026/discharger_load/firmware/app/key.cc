#include "misc.h"
#include "platform.h"
#include "key.h"
#include "zkey.h"

static int key_read(int key_id);
static void key_action(int key_id, int action);

static ZKey::cbs_t cbs = {key_read, key_action};

ZKey zk(cbs, 2, 300, 1);

static int key_read(int key_id)
{
  if(ZPin::read(PA0) == 0 && key_id == 0)
    return 1;
  else if(ZPin::read(PA1) == 0 && key_id == 1)
    return 1;
  return 0;
}

static void key_action(int key_id, int action)
{
  switch(action) {
    case zk.PRESS:
      ze.post(events::KEY_PRESS, key_id);
      break;
    case zk.LONGPRESS:
      ze.post(events::KEY_LONGPRESS, key_id);
      break;
//    case zk.CLICK: ze.post(events::KEY_CLICK, key_id); break;
  }
}

void KEY_Config(void)
{
  // PB8/9 as keys
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);

  print_log();
}
