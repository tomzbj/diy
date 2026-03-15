#include "misc.h"
#include "platform.h"

ZTASK zt(20, millis);

void task_uart(void)
{
  if(u1.idle()) {
    char buf[128] = {0};
    int len = u1.read(buf, sizeof(buf));
    cli.parse(buf, len);
  }
}

void task_hello(void)
{
  printf("Hello, world.\n");
//  printf("%d\n", u1.available());
}

void TASKS_Config(void)
{
  zt.bind(task_uart, 1, true);
  zt.bind(task_hello, 250, false);
  zt.bind([] {mod.poll();}, 1, true);

  print_log();
}
