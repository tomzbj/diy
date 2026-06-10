/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef CHERRYUSB_CONFIG_H
#define CHERRYUSB_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

  /* ================ USB common Configuration ================ */

#define CONFIG_USB_PRINTF(...) //printf(__VA_ARGS__)

#define usb_malloc(size) malloc(size)
#define usb_free(ptr)    free(ptr)

#ifndef CONFIG_USB_DBG_LEVEL
#define CONFIG_USB_DBG_LEVEL USB_DBG_ERROR
#endif

  /* Enable print with color */
#define CONFIG_USB_PRINTF_COLOR_ENABLE

  /* data align size when use dma */
#ifndef CONFIG_USB_ALIGN_SIZE
#define CONFIG_USB_ALIGN_SIZE 4
#endif

  /* attribute data into no cache ram */
#define USB_NOCACHE_RAM_SECTION __attribute__((section(".noncacheable")))

  /* ================= USB Device Stack Configuration ================ */

  /* Ep0 max transfer buffer, specially for receiving data from ep0 out */
#define CONFIG_USBDEV_REQUEST_BUFFER_LEN 256

  /* Setup packet log for debug */
// #define CONFIG_USBDEV_SETUP_LOG_PRINT
  /* Check if the input descriptor is correct */
// #define CONFIG_USBDEV_DESC_CHECK
  /* Enable test mode */
// #define CONFIG_USBDEV_TEST_MODE
#ifndef CONFIG_USBDEV_MSC_BLOCK_SIZE
#define CONFIG_USBDEV_MSC_BLOCK_SIZE 512
#endif

#ifndef CONFIG_USBDEV_MSC_MANUFACTURER_STRING
#define CONFIG_USBDEV_MSC_MANUFACTURER_STRING ""
#endif

#ifndef CONFIG_USBDEV_MSC_PRODUCT_STRING
#define CONFIG_USBDEV_MSC_PRODUCT_STRING ""
#endif

#ifndef CONFIG_USBDEV_MSC_VERSION_STRING
#define CONFIG_USBDEV_MSC_VERSION_STRING "0.01"
#endif

// #define CONFIG_USBDEV_MSC_THREAD

#ifdef CONFIG_USBDEV_MSC_THREAD
#ifndef CONFIG_USBDEV_MSC_STACKSIZE
#define CONFIG_USBDEV_MSC_STACKSIZE 2048
#endif

#ifndef CONFIG_USBDEV_MSC_PRIO
#define CONFIG_USBDEV_MSC_PRIO 4
#endif
#endif

#ifndef CONFIG_USBDEV_AUDIO_VERSION
#define CONFIG_USBDEV_AUDIO_VERSION 0x0100
#endif

#ifndef CONFIG_USBDEV_AUDIO_MAX_CHANNEL
#define CONFIG_USBDEV_AUDIO_MAX_CHANNEL 8
#endif

  /* ================ USB Device Port Configuration ================*/
#include "py32f0xx_hal.h"

#define USBD_IRQn       USB_IRQn

#define USBD_IRQHandler USBD_IRQHandler

  void cdc_acm_init(void);
  void cdc_acm_data_send_with_dtr_test(void);
  void cdc_acm_data_send(uint8_t* data, int size);

#ifdef __cplusplus
}
#endif

#endif
