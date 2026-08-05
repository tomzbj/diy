#include "misc.h"
#include "platform.h"

const ZUART4::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUART4 u1(cbs_u1, 512);

const CHARGER::CBS cbs = {    //
  .adc_get_f = ADC_Get, .set_voltcurr_f = TIMER15_SetVoltCurr};
CHARGER charger(cbs);

int main(void)
{
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  USART1_Config();
  DISP_Config();
  TIMER15_Config();
  ADC_Config();
  CLI_Config();
  TASKS_Config();

  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());

  while(1) {
    _delay_us(1);
    zt.poll();
  }
  return 0;
}
