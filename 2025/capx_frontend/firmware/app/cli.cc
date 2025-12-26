#include "misc.h"
#include "platform.h"

auto ok = +[]() {printf("ok\n");};

ZCli cli(20);

static void cli_rd_pid(const char** tokens)
{
  ZPID::PID pid = zpid.get_pid();
  printf("p, i, d, lime: %.3f %.3f %.3f %.3f\n", pid.p, pid.i, pid.d, pid.lime);
}

static void cli_wr_sp(const char** tokens)
{
  int sp = atoi(tokens[2]);
  zpid.set_sp(sp*100);
  ok();
}


void CLI_Config(void)
{
  cli.bind("test", [](const char**) {puts("TEST OK");});
  cli.bind("reboot", [](const char**) {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd",
    [](const char**) {printf("%lu\n", HAL_RCC_GetSysClockFreq());});

  cli.bind("adc", "rd", [](const char**) {printf("%u\n", ADC_Get());});
  cli.bind("cnt", "rd", [](const char**) {printf("%lu\n", TIM1->CNT);});
  cli.bind("freq_get", "rd", [](const char**) {printf("%lu\n", counter.get());});
  cli.bind("temp", "rd", [](const char**) {printf("%d\n", lm75.read_temp());});
  cli.bind("pid", "rd", cli_rd_pid);
  cli.bind("output", "rd", [](const char**) {printf("%d\n", zpid.get_output());});
  cli.bind("sp", "wr", cli_wr_sp);

  print_log();
}
