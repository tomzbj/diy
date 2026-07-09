#include "misc.h"
#include "platform.h"
#include "tusb.h"

extern "C" void OTG_FS_IRQHandler(void)
{
  tud_int_handler(0);    // 0 代表第一個 USB 控制器
}

extern "C" void USB_Config(void) { __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_NVIC_SetPriority(OTG_FS_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ (OTG_FS_IRQn);

  GPIO_InitTypeDef gis = {0};

  gis.Pin = GPIO_PIN_11 | GPIO_PIN_12;
  gis.Speed = GPIO_SPEED_HIGH;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &gis);

  HAL_Delay(1);

  // init device stack on configured roothub port
  tusb_rhport_init_t dev_init = {.role = TUSB_ROLE_DEVICE, .speed = TUSB_SPEED_AUTO};
  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  print_log();
}

extern "C" void tud_umount_cb(void)
{
  mtp_host_disconnected();
}
