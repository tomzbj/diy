#include "misc.h"
#include "platform.h"

ZTASK zt(20, millis);

void task_uart(void)
{
#if APP_USART_ENABLE
  if(u1.available() > 0) {
    if(u1.peek() == '#' && u1.idle()) {
      char buf[128] = {0};
      int len = u1.read(buf, sizeof(buf));
      cli.parse(buf, len);
    }
  }
#endif
}

void task_hello(void)
{
  printf("Hello, world.\n");
}

void TASKS_Config(void)
{
#if APP_USART_ENABLE
  zt.bind(task_uart, 1, true);
#endif
  zt.bind(task_hello, 250, false);
  zt.bind(ALARM_Task, 50, true);     // blink + LVGL only while alarming
  zt.bind(UI_Task, 100, true);       // normal countdown: 10Hz LVGL
  zt.bind(MEAS_Poll, 250, true);
  zt.bind(BAT_Poll, 1000, true);     // VBAT icon ~1Hz

  print_log();
}
