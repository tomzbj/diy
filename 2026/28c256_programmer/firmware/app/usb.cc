#include "misc.h"
#include "platform.h"

extern "C" void USB_IRQHandler(void)
{
  void USBD_IRQHandler(uint8_t busid);
  USBD_IRQHandler(0);
}

extern "C" void usb_dc_low_level_init(int busid)
{
}

extern "C" void USB_Read(uint8_t* data, int size)
{
  ucdc.push(data, size);
}

extern "C" void USB_Write(uint8_t* data, int size)
{
  void cdc_acm_data_send(uint8_t* data, int size);
  cdc_acm_data_send(data, size);
}

extern "C" void USB_Config(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_USB_CLK_ENABLE();
//  msc_ram_init();
  void cdc_acm_init(void);
  cdc_acm_init();
  HAL_NVIC_SetPriority(USB_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ (USB_IRQn);

  HAL_Delay(100);
  print_log();
}
