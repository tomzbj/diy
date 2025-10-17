#include "misc.h"
#include "platform.h"
#include "key.h"
#include "zkey.h"

static int key_read(int key_id);
static void key_action(int key_id, int action);

static ZKey::cbs_t cbs = {key_read, key_action};

ZKey zk(cbs, 5, 300, 1);

static int key_read(int key_id)
{
  zpin_t key[] = {PB5, PB6, PB7, PB8, PB9};
  if(0) {
    static int count = 0;
    ++count %= 1000;
    if(count == 0)
      printf("%d\n", key_id);
  }
  for(int i = 0; i < 5; i++) {
    if(ZPin::read(key[key_id]) == 0)
      return 1;
  }
  return 0;
}

static void key_action(int key_id, int action)
{
  ZEVENT::evt_t evt;
  if(action != ZKey::NONE) {
    switch(action) {
      case ZKey::CLICK:
        evt.id = EVENTS::KEY_CLICKED;
        break;
      case ZKey::PRESS:
        evt.id = EVENTS::KEY_PRESSED;
        break;
      case ZKey::LONGPRESS:
        evt.id = EVENTS::KEY_LONGPRESSED;
        break;
      case ZKey::DOUBLECLICK:
        evt.id = EVENTS::KEY_DOUBLECLICKED;
        break;
    }
    evt.data = key_id;
    ze.post(evt);
  }
}

void KEY_Config(void)
{
  // PB5&6 as SW1&2, PB7/8/9 as thumbwheel
  rcu_periph_clock_enable (RCU_GPIOB);
  auto pins = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
  gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pins);

  print_log();
}
