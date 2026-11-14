#include "misc.h"
#include "platform.h"

ZTASK zt(20, millis);

static void task_uart(void)
{
  if(u1.idle()) {
    char buf[128] = {0};
    char c = u1.peek();
    int len = u1.read(buf, sizeof(buf));
    if(c == '#')
      cli.parse(buf, len);
  }
}

void TASKS_Config(void)
{
//  zt.bind([] {printf("Hello, world.\n");}, 500, true);
  zt.bind(task_uart, 1, true);
//  zt.bind([] {zk.poll();}, 1, true);
  zt.bind([]() {zk.scan();}, 1, true);
  zt.bind([]() {DISP_Update();}, 500, true);

  print_log();
}
