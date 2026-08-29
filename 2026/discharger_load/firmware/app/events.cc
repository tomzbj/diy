#include "misc.h"
#include "platform.h"
#include "events.h"

ZEVENT ze(32, 32);

static void keypress_f(uint32_t data)
{
  printf("keypressed %lu\n", data);
//  DISP_Update();
  if(data == 1)
    meas.key_up_press();
  else if(data == 0)
    meas.key_down_press();
}

static void keyclick_f(uint32_t data)
{
//  printf("keyclicked %lu\n", data);
//  DISP_Update();
}

static void keylongpress_f(uint32_t data)
{
  printf("keylongpressed %lu\n", data);
  if(data == 1)
    meas.key_up_longpress();
  else if(data == 0)
    meas.key_down_longpress();
}

void EVENTS_Config(void)
{
  ze.bind(events::KEY_PRESS, keypress_f);
  ze.bind(events::KEY_LONGPRESS, keylongpress_f);
  ze.bind(events::KEY_CLICK, keyclick_f);

  print_log();
}
