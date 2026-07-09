#include "misc.h"
#include "platform.h"

ZTASK zt(20, millis);

static void task_uart(void)
{
  if(u1.idle()) {
//_dbg();
    char buf[128] = {0};
    int len = u1.read(buf, sizeof(buf));
//printf("%d %s\n", len, buf);
    cli.parse(buf, len);
  }
}

static void task_hello(void)
{
  printf("Hello, world.\n");
}

void TASKS_Config(void)
{
  zt.bind(task_uart, 1, true);
  zt.bind(task_hello, 500, false);
  zt.bind([]() {player.poll();}, 1, true);
}

