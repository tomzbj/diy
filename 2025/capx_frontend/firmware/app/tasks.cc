#include "misc.h"
#include "platform.h"

ZTASK zt(20, millis);

void task_led(void)
{
  printf("Hello, world.\n");
}

void task_uart(void)
{
  if(u1.idle()) {
    char buf[128] = {0};
    char c = u1.peek();
    int len = u1.read(buf, sizeof(buf));
    if(c == '#') {
      cli.parse(buf, len);
    }
  }
  /* if(u2.idle()) {
   char buf[1024] = {0}; //      char c = u2.peek();
   int len = u2.read(buf, sizeof(buf)); //      u1.write(buf, len);
   bds.parse(buf, len); }*/
}

static void task_tempctl(void)
{
  zpid.poll();
  const char modes[] = "SAPO";
  int mode = static_cast<int>(zpid.get_mode());

  char buf[64] = {0};

  uint32_t temp = lm75.read_temp();
  auto freq = counter.get();
  sprintf(buf, "$%c%04lu%08lu\n", modes[mode], temp, freq);
  u1.write(buf, strlen(buf));
//  u1.write(buf, strlen(buf));
}

void TASKS_Config(void)
{
  zt.bind(task_uart, 1, true);
  zt.bind(task_led, 250, false);
  zt.bind(task_tempctl, 100, true);
}
