#include "misc.h"
#include "platform.h"

const ZUART4::CBS cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUART4 u1(cbs_u1, 512);

int main(void)
{
  DWT_ON();
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  u1.set_timeout(20);
  USART1_Config();    //  printf("[%s: %d] USART Initialized.\n", __FILE__, __LINE__);
  CLI_Config();
  TIMER6_Config();    // for dac
  TIMER3_Config();    // for adc
  TIMER2_Config();    // for lcd bk
  DAC_Config();
  ADC_Config();
  RTC_Config();
  KEY_Config();
  MOTOR_Config();
  if(1) {
    SPI3_Config();
    DISP_Config();
  }
  TASKS_Config();
//  IWDG_Config();
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());    //  printf("All peripherals initialized.\n");

  while(1) {
//    IWDG_Feed();
    _delay_us(1);
    zt.poll();
  }
  return 0;
}
