#include "misc.h"
#include "platform.h"

#define ok() printf("ok\n")

void CLI_Config(void)
{
  auto test_f = +[](const char**) {puts("TEST OK");};
  auto reboot_f = +[](const char**) {puts("Reboot..."); NVIC_SystemReset();};

  cli.bind("test", test_f);
  cli.bind("reboot", reboot_f);
}
