#include "misc.h"
#include "platform.h"
#include "events.h"

ZEVENT ze(32, 32);

static void keypress_f(uint32_t data)
{
  printf("keypressed %lu\n", data);
//  DISP_Update();
//  if(data == 0) meas.toggle_running_state(); else if(data == 1) meas.toggle_params();
}

static void keyclick_f(uint32_t data)
{
//  printf("keyclicked %lu\n", data);
//  DISP_Update();
}

static void keylongpress_f(uint32_t data)
{
  printf("keylongpressed %lu\n", data);
//  DISP_Update();
//  if(data == 1) meas.toggle_params_mode();
}

void EVENTS_Config(void)
{
  ze.bind(events::KEY_PRESS, keypress_f);
  ze.bind(events::KEY_LONGPRESS, keylongpress_f);
  ze.bind(events::KEY_CLICK, keyclick_f);

  print_log();
}
