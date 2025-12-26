#include "platform.h"

const ZUart3::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUart3 u1(cbs_u1, 512);

const ZUart3::cbs_t cbs_u2 = {    //
  .write_f = USART2_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUart3 u2(cbs_u2, 1024);

void BSP_Init(void)
{
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  USART1_Config();
//  USART2_Config();
  CLI_Config();

//  ADC_Config();
  TIMER1_Config();    // freq counter
  TIMER14_Config();    // pwm
  TIMER16_Config();    // 0.1s gate
  SENSOR_Config();
  TASKS_Config();
  TEMPCTL_Config();

  print_log();
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());
}
