#include "misc.h"
#include "platform.h"

ZCli cli(8);

#define ok() printf("ok\n");

static void cli_wr_duty(const char* tokens[])
{
  int channel = atoi(tokens[2]);
  int duty = atoi(tokens[3]);
  TIMER15_SetDuty(channel, duty);
  printf("%lu %lu\n", TIM15->CCR1, TIM15->CCR2);
  ok();
}

static void cli_rd_adc(void)
{
//  volatile uint32_t t1 = millis();
  volatile uint32_t t2 = SysTick->VAL;
  float volt = 0, curr = 0;
  ADC_Get(&volt, &curr);
  volatile uint32_t t1 = SysTick->VAL;
  printf("%8dmV %8dmA %8lu\n", int(volt * 1000), int(curr), t2 - t1);
}

static void cli_wr_voltcurr(const char* tokens[])
{
  float volt = atof(tokens[2]);
  float curr = atof(tokens[3]);
  TIMER15_SetVoltCurr(volt, curr);
  ok();
}

void CLI_Config(void)
{
  cli.bind("test", []() {puts("TEST OK");});
  cli.bind("reboot", []() {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd", []() {printf("%lu\n", HAL_RCC_GetSysClockFreq());});
  cli.bind("duty", "wr", cli_wr_duty);
  cli.bind("adc", "rd", cli_rd_adc);
  cli.bind("voltcurr", "wr", cli_wr_voltcurr);
  print_log();
}
