#include "misc.h"
#include "platform.h"

auto ok = +[]() {printf("ok\n");};

ZCli cli(20);

void CLI_Config(void)
{
  cli.bind("test", [](const char**) {puts("TEST OK");});
  cli.bind("reboot", [](const char**) {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd",
    [](const char**) {printf("%lu\n", HAL_RCC_GetSysClockFreq());});
  cli.bind("adc", "rd", [](const char**) {printf("%u\n", ADC_Get());});

  print_log();
}
