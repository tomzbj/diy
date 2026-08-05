#include "misc.h"
#include "platform.h"

ZTASK zt(20, millis);

void task_uart(void)
{
  char buf[128] = {0};
  if(u1.available()) {
    char c = u1.peek();
    if(c == '#' && u1.idle()) {
      int len = u1.read(buf, sizeof(buf));
      cli.parse(buf, len);
    }
  }
}

void TASKS_Config(void)
{
  zt.bind(task_uart, 1, true);
  zt.bind([]() {printf("Hello, world.\n");}, 250, false);
  zt.bind(DISP_Update, 1000, true);
  zt.bind([]() {charger.poll();}, 1000, true);
}
