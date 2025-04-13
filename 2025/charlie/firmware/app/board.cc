#include "misc.h"
#include "platform.h"

const ZUart3::cbs_t cbs_u0 = {    //
//  .write_f = USART0_Write_DMA, .pre_tx_f = nullptr, .post_tx_f = nullptr};
  .write_f = USART0_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUart3 u0(cbs_u0, 1024);
ZCli cli(20);
CHARLIE charlie;

void BSP_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;

  SysTick_Config(rcu_clock_freq_get(CK_SYS) / 1000UL);

  USART_Config();
  CLI_Config();
  charlie.init();
  TIMER_Config();
  DISP_Config();

  print_log();
  printf("SYSCLK: %lu\n", rcu_clock_freq_get(CK_SYS));
}
