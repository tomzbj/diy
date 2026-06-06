#include "misc.h"
#include "platform.h"
#include "key.h"
#include "zkey.h"
#include "ui.h"

static int key_read(int key_id);
static void key_action(int key_id, int action);

static ZKey::cbs_t cbs = {key_read, key_action};

ZKey zk(cbs, 2, 150, 1);

static int key_read(int key_id)
{
  if(ZPin::read(PB10) == 0 && key_id == 0)
    return 1;
  if(ZPin::read(PB11) == 0 && key_id == 1)
    return 1;
  return 0;
}

static void key_action(int key_id, int action)
{
  extern class UI ui;
  switch(action) {
    case zk.PRESS:
      if(key_id == 0)
        ui.send_key(UI::KEY::UP_PRESS);
      else if(key_id == 1)
        ui.send_key(UI::KEY::DOWN_PRESS);
      break;
    case zk.LONGPRESS:
      if(key_id == 0)
        ui.send_key(UI::KEY::UP_LONGPRESS);
      else if(key_id == 1)
        ui.send_key(UI::KEY::DOWN_LONGPRESS);
      break;
  }
}

void KEY_Config(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // PB10&11 as key
  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_10 | GPIO_PIN_11;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);
  print_log();
}
