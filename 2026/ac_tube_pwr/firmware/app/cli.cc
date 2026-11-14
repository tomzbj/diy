#include "misc.h"
#include "platform.h"

auto ok = +[]() {printf("ok\n");};

ZCli cli(20);

static void cli_rd_adc(void)
{
  ADC_RESULT* result = ADC_Get();
  printf("%8.3f %8.3f %8.3f %8.3f\n",    //
    result->volt_a, result->curr_a, result->volt_b, result->curr_b);
}

void CLI_Config(void)
{
  cli.bind("test", []() {puts("TEST OK");});
  cli.bind("reboot", []() {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd", []() {printf("%lu\n", HAL_RCC_GetSysClockFreq());});
  cli.bind("adc", "rd", cli_rd_adc);

//  cli.bind("meas", "rd", MEAS_Poll);

  print_log();
}
