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
//  zt.bind([]() {printf("Hello, world.\n");}, 250, true);
  zt.bind([]() {rom.update();}, 1, true);
  zt.bind([]() {zk.poll();}, 1, true);
//  zt.bind(DISP_Update, 500, true);
}
