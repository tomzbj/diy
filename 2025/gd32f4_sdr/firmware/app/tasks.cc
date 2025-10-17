#include "misc.h"
#include "platform.h"

/* @formatter:off */
void task_led(void)
{
  task_open() ;
  while(1) {
    ZPin::toggle (PB0);
    task_wait(100);
  }
  task_close() ;
}

void task_uart(void)
{
  task_open() ;
  while(1) {
    if(u0.idle()) {
//_dbg();
      char buf[1024];
      memset(buf, 0, sizeof(buf));
      char c = u0.peek();
      int len = u0.read(buf, sizeof(buf));
      if(c == '#') cli.parse(buf, len);
    }
    task_wait(1);
  }
  task_close() ;
}
/* @formatter:on*/

void TASKS_Config(void)
{
  task_create(task_uart, NULL, 1, NULL, 0, 0);

  SysTick_Config(rcu_clock_freq_get(CK_SYS) / 1000UL);
  os_start();
}
