#include "platform.h"

const ZUart3::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
static uint8_t rxfifo_u1[128];
ZUart3 u1(cbs_u1, rxfifo_u1, sizeof(rxfifo_u1));
FAN fan;

void BSP_Init(void)
{
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);

  USART_Config();
  CLI_Config();
  KEY_Config();
  TIMER_Config();
//  LPTIM_Config();
  fan.init();

  ze.post(EVENTS::KEY_LONGPRESSED);
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());
}
