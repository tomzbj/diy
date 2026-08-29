#include "misc.h"
#include "platform.h"

auto ok = +[]() {printf("ok\n");};

ZCli cli(20);

static void cli_poll(void)
{
  auto t1 = millis();
  DISP_Update();
  auto t2 = millis();
  printf("%lu ms\n", t2 - t1);
}

static void cli_wr_duty(const char* tokens[])
{
  TIMER3_SetDuty(atoi(tokens[2]));
  ok();
}

static void cli_rd_adc(void)
{
  float volt, curr;
  volatile int t1, t2;
  t1 = millis();
  ADC_Get(&volt, &curr);
  t2 = millis();
  printf("%8dmV  %8dmA %d\n", int(volt * 1000), int(curr), t2 - t1);
}

void CLI_Config(void)
{
  cli.bind("test", []() {puts("TEST OK");});
  cli.bind("reboot", []() {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd", []() {printf("%lu\n", HAL_RCC_GetSysClockFreq());});
  cli.bind("adc", "rd", cli_rd_adc);
  cli.bind("poll", cli_poll);
  cli.bind("duty", "wr", cli_wr_duty);

  print_log();
}
