#include "misc.h"
#include "platform.h"

ZCli cli(30);

const ZUart3::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUart3 u1(cbs_u1, 128);

void BSP_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;

  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  u1.set_timeout(5);
  USART1_Config();
  CLI_Config();
  TIMER3_Config();
  DAC_Config();
  TIMER6_Config();
  DDS_Config();
  ADC_Config();
  TASKS_Config();

  print_log();
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());
}
