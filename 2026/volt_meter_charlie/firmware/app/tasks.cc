#include "misc.h"
#include "platform.h"

ZTASK zt(20, millis);

static void task_uart(void)
{
  if(u1.available() > 0) {
    char c = u1.peek();
    if(c == '#' && u1.idle()) {
      char buf[128] = {0};
      int len = u1.read(buf, sizeof(buf));
      cli.parse(buf, len);
    }
    else if(u1.idle()) {
      u1.clear();
    }
  }
}

static void task_disp(void)
{
  static float n = 0.0001;
  n *= 1.1;
  if(n > 1500)
    n = 0.0001;
  charlie.write(n);
}

static void task_meas(void)
{
  auto val = ADC_Get();
  float volt = val / 4095.0 * 3.3 * 21;
  charlie.write(volt);
}

void TASKS_Config(void)
{
  zt.bind(task_uart, 1, true);
//  zt.bind(DISP_Poll, 1, true);
  zt.bind(task_disp, 250, false);
  zt.bind(task_meas, 250, true);
  print_log();
}
