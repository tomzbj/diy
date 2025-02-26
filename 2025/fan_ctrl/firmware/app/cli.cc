#include "misc.h"
#include "platform.h"

auto ok = +[]() {printf("ok\n");};

ZCli cli(20);

static void fan(const char* tokens[])
{
  uint32_t s = atoi(tokens[2]);
  if(s > 990)
    s = 990;
  TIMER_SetPWM(s);
  ZPin::write(PB1, s);
  ok();
}

void CLI_Config(void)
{
  cli.bind("test", [](const char**) {puts("TEST OK");});
  cli.bind("reboot", [](const char**) {puts("REBOOT."); NVIC_SystemReset();});
//  cli.bind("freq", "rd", [](const char**) {printf("%lu\n", HAL_RCC_GetSysClockFreq());});
  cli.bind("freq", "rd", [](const char**) {printf("%lu\n", SystemCoreClock);});
  cli.bind("fan", "wr", fan);

  print_log();
}
