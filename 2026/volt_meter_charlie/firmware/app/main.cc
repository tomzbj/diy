#include "platform.h"

const ZUART4::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUART4 u1(cbs_u1, 32);

int main(void)
{
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  USART_Config();
  CLI_Config();
  TASKS_Config();
  DISP_Config();
  ADC_Config();

  print_log();
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());

  while(1) {
    _delay_us(1);
    zt.poll();
  }
  return 0;
}
