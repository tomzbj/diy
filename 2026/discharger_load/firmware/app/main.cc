#include "platform.h"

const ZUART4::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUART4 u1(cbs_u1, 128);
//DISP disp;

const MEAS::CBS cbs_meas = {    //
  .millis_f = millis, .adc_get_f = ADC_Get, .set_curr_f = TIMER3_SetCurr};
MEAS meas(cbs_meas);

int main(void)
{
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);

  u1.set_timeout(2);
  USART1_Config();

  CLI_Config();
  ADC_Config();
  TIMER3_Config();
  DISP_Config();
//  disp.init();
  KEY_Config();
  EVENTS_Config();

  TASKS_Config();
  print_log();
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());

  while(1) {
//    _delay_us(1);
    zt.poll();
    ze.poll();
  }
  return 0;
}
