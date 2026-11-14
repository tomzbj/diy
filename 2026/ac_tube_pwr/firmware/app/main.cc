#include "platform.h"

const ZUART4::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUART4 u1(cbs_u1, 128);

int main(void)
{
  SystemCoreClock = 48'000'000UL;
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  USART1_Config();
  CLI_Config();
  TIMER1_Config();
  ADC_Config();
  //  MEAS_Config();
  KEY_Config();
  DISP_Config();
  TASKS_Config();
  print_log();
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());
  while(1) {
    zt.poll();
    zk.poll();
  }
  return 0;
}
