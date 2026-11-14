#include "misc.h"
#include "platform.h"
#include "key.h"
#include "zkey.h"

static int key_read(int key_id);

ZKEY zk(key_read, 1, 300);

static int key_read(int key_id)
{
  if(ZPin::read(PB3) == 0)
    return 1;
  return 0;
}

static void on_press(int key_id)
{
  printf("on press\n");
  TIMER1_AdjustMod();
}

static void on_longpress(int key_id)
{
  printf("on longpress\n");
  TIMER1_Zero();
}

void KEY_Config(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_3;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);

  zk.bind(ZKEY::PRESS, on_press);
  zk.bind(ZKEY::LONGPRESS, on_longpress);

  print_log();
}
