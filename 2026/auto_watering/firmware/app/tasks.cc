#include "misc.h"
#include "platform.h"
#include "ui.h"
#include <malloc.h>

ZTASK zt(20, millis);
uint32_t now = 0;
uint32_t motor_duration = 3;

static void task_uart(void)
{
  if(u1.available() > 0) {
    if(u1.idle() && u1.peek() == '#') {
      char buf[512] = {0};
      int len = u1.read(buf, sizeof(buf));
//      printf("%d %s\n", len, buf);
      cli.parse(buf, len);
    }
  }
}

static void task_disp(void)
{
  extern class UI ui;
  ui.update();
  lv_tick_inc(30);
  lv_timer_handler();
}

static void task_hello(void)
{
  printf("Hello, world.\n");

  register uint8_t* sp asm("sp");
  auto a2 = sp;
  auto a1 = malloc(1);
  printf("%08lx %08lx\n", (uint32_t)a1, (uint32_t)a2);
  if(a2 != nullptr)
    free(a1);

  struct mallinfo mi = mallinfo();
  printf("Used: %d, Free: %d\n", mi.uordblks, mi.fordblks);

  {
    struct _reent* r = _impure_ptr;
    printf("Reent pointer: %08lx\n", (uint32_t)(void*)r);
    printf("Reent size: %d\n", sizeof(struct _reent));

    // 如果 _stdout 不为空，说明标准 I/O 已经初始化并占用了内存
    if(r->_stdout != NULL) {
      printf("stdout is initialized at %08lx\n", (uint32_t)((void*)r->_stdout));
    }
  }
}

static void task_vbatmon(void)
{
  auto vbat = ADC_GetVbat();
  if(vbat > 3100)
    return;

  HAL_RCC_DeInit();
  USART1_Config();

  while(ADC_GetVbat() < 3200) {
    TIM2->CCR1 = 0;
    HAL_Delay(100);
  }
  NVIC_SystemReset();
}

static void task_motor(void)
{
  now++;
  if(motor_duration > 0) {
    motor_duration--;
    if(motor_duration == 0)
      MOTOR_Set(MOTOR_STOP);
  }
  extern class UI ui;
//  printf("%8lu %8lu %8lu\n", now, ui.cfgs.time_next, ui.cfgs.duration);
  if(now == ui.cfgs.time_next) {
    MOTOR_Set(MOTOR_RUN);
    motor_duration = ui.cfgs.duration;
  }
}

void TASKS_Config(void)
{
//  now = (time(0) + 8 * 3600) % 86400;
  zt.bind(task_uart, 1, true);
  zt.bind(task_hello, 1250, false);
  zt.bind(task_disp, 30, true);
  zt.bind(task_motor, 1000, true);
  zt.bind([] {zk.poll();}, 1, true);
  zt.bind(task_vbatmon, 1000, true);

  print_log();
}
