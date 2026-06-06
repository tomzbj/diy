#include "misc.h"
#include "app.h"
#include <malloc.h>
#include "windows.h"
#include <ctime>

uint32_t now;
auto millis = +[](void) -> uint32_t {return GetTickCount();};
ZTASK zt(20, millis);

static void task_uart(void)
{
  extern class ZUART4 udp;
  udp.tick();
  extern class ZCli cli;
  if(udp.idle()) {
    char buf[4096] = {0};
    char c = udp.peek();
    int len = udp.read(buf, sizeof(buf));
    if(c == '#')
      cli.parse(buf, len);
    else
      udp.write(buf, len);
  }
}

static void task_disp(void)
{
  fflush(stdout);
  lv_tick_inc(16);
  lv_timer_handler();
  if(1) {
    static int count = 0;
    ++count %= 10;
    if(!count) {
      extern class UI ui;
      ui.update();
    }
  }
}

static void task_hello(void)
{
  printf("Hello, world.\n");
  register uint8_t* sp asm("sp");
  auto a2 = sp;
  auto a1 = malloc(1);
  printf("%08x %08x\n", (uint32_t)a1, (uint32_t)a2);
  if(a2 != nullptr)
    free(a1);
}

static void task_time(void)
{
  now++;
}

void TASKS_Config(void)
{
  {
    now = (time(0) + 8 * 3600) % 86400;
  }
  zt.bind(task_uart, 1, true);
  zt.bind(task_hello, 1250, false);
  zt.bind(task_disp, 16, true);
  zt.bind(task_time, 1000, true);
  _dbg();
}
