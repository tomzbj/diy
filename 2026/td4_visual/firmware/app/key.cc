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
  if(ZPin::read(PF7) == 0 && key_id == 0)
    return 1;
  else if(ZPin::read(PF6) == 0 && key_id == 1)
    return 1;
  return 0;
}

static void key_action(int key_id, int action)
{
  switch(action) {
    case zk.PRESS:
      if(key_id == 0)
        disp.go(DISP::KEY_UP_PRESS);
      else if(key_id == 1)
        disp.go(DISP::KEY_DOWN_PRESS);
//      printf("PRESS %d\n", key_id);
//      ze.post(events::KEY_PRESS, key_id);
      break;
    case zk.LONGPRESS:
      if(key_id == 0)
        disp.go(DISP::KEY_UP_LONGPRESS);
      else if(key_id == 1)
        disp.go(DISP::KEY_DOWN_LONGPRESS);

//      printf("LONGPRESS %d\n", key_id);
//      ze.post(events::KEY_LONGPRESS, key_id);
      break;
//    case zk.CLICK: ze.post(events::KEY_CLICK, key_id); break;
  }
}

void KEY_Config(void)
{
  __HAL_RCC_GPIOF_CLK_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &gis);

  print_log();
}
