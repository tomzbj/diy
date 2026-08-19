#include "misc.h"
#include "platform.h"

ZCli cli(20);

const ZUART4::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUART4 u1(cbs_u1, 128);

int main(void)
{
  PWR_EN_LDO();
  DWT_ON();
  SystemCoreClock = HAL_RCC_GetSysClockFreq();
  SysTick_Config(SystemCoreClock / 1000UL);
#if APP_USART_ENABLE
  u1.set_timeout(5);
  USART1_Config();
  CLI_Config();
#endif
  TIMER3_Config();
  TIMER5_Config();

  ADC_Config();
  SPI1_Config();
  DISP_Config();
  UI_Config();
  MEAS_Config();
  BAT_Config();
  TASKS_Config();

  print_log();
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());

  while(1) {
    zt.poll();
    __WFI();    // sleep until SysTick / IRQ; cuts idle MCU current
  }
}
