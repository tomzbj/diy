#include "misc.h"
#include "tempctl.h"
#include "platform.h"
#include "zpid.h"

static float get_temp_f(void);
static void output_f(int val);

ZPID::CBS cbs = {get_temp_f, output_f};
ZPID zpid(cbs, 0, 990, 100);

static float get_temp_f(void)
{
  extern class LM75 lm75;
  float temp = lm75.read_temp();
  return temp;
}

static void output_f(int val)
{
  TIM14->CCR1 = val;
}

void TEMPCTL_Config(void)
{
  zpid.set_pid( {534, 53, 800, 0.5});
  zpid.set_sp(4500);
  zpid.set_mode(zpid.MODE::AUTO_TUNE);
}

