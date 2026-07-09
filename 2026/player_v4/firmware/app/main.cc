#include "misc.h"
#include "platform.h"
#include "tusb.h"
#include "usb.h"
#include "player.h"

const ZUART4::cbs_t cbs_u1 =    //
  {.write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};

ZUART4 u1(cbs_u1, 128);
int main(void)
{
  SystemCoreClock = HAL_RCC_GetSysClockFreq();
  DWT_ON();
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  u1.set_timeout(10);
  USART1_Config();    //  printf("[%s: %d] USART Initialized.\n", __FILE__, __LINE__);
  CLI_Config();
  BSP_SD_Init();

  FLASH_Config();
  USB_Config();
  I2S_Config();
  TASKS_Config();
  AUDIO_Config();

  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());
  printf("All peripherals initialized.\n");

  while(1) {
    tud_task();
    mtp_upload_poll();
    zt.poll();
  }
  return 0;
}
