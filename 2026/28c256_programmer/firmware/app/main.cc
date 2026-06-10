#include "platform.h"

void USB_Write(uint8_t* data, int size);

const ZUART4::cbs_t    //
cbs_u1 = {.write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr},    //
  cbs_ucdc = {.write_f = USB_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};

ZUART4 u1(cbs_u1, 2048), ucdc(cbs_ucdc, 2048);

int main(void)
{
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);

  USB_Config();

  USART1_Config();
  EEPROM_Config();
  CLI_Config();
  TASKS_Config();
  print_log();
  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());

  while(1) {
//    _delay_us(1);
    zt.poll();
  }
  return 0;
}
