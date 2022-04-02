#ifndef __USB_CONF__H__
#define __USB_CONF__H__

#include "stm32f4xx.h"

#define USE_USB_OTG_FS

#ifndef USE_USB_OTG_FS
#endif 
#ifdef USE_USB_OTG_FS
#define USB_OTG_FS_CORE
#endif
#ifndef USE_USB_OTG_HS
#endif 
#ifndef USE_ULPI_PHY
#endif 
#ifndef USE_EMBEDDED_PHY
#endif 
#ifdef USE_USB_OTG_HS
#define USB_OTG_HS_CORE
#endif
#ifdef USB_OTG_HS_CORE
#define RX_FIFO_HS_SIZE                          512
#define TX0_FIFO_HS_SIZE                         128
#define TX1_FIFO_HS_SIZE                         272
#define TX2_FIFO_HS_SIZE                          0
#define TX3_FIFO_HS_SIZE                          0
#define TX4_FIFO_HS_SIZE                          0
#define TX5_FIFO_HS_SIZE                          0
#ifdef USE_ULPI_PHY
#define USB_OTG_ULPI_PHY_ENABLED
#endif
#ifdef USE_EMBEDDED_PHY
#define USB_OTG_EMBEDDED_PHY_ENABLED
#endif
#define USB_OTG_HS_DEDICATED_EP1_ENABLED
#endif
#ifdef USB_OTG_FS_CORE
#define RX_FIFO_FS_SIZE                          128
#define TX0_FIFO_FS_SIZE                          64
#define TX1_FIFO_FS_SIZE                         128
#define TX2_FIFO_FS_SIZE                          0
#define TX3_FIFO_FS_SIZE                          0
#endif
//#define VBUS_SENSING_ENABLED
#define USE_DEVICE_MODE
#ifndef USB_OTG_FS_CORE
#ifndef USB_OTG_HS_CORE
#error  "USB_OTG_HS_CORE or USB_OTG_FS_CORE should be defined"
#endif
#endif
#ifndef USE_DEVICE_MODE
#ifndef USE_HOST_MODE
#error  "USE_DEVICE_MODE or USE_HOST_MODE should be defined"
#endif
#endif
#ifndef USE_USB_OTG_HS
#ifndef USE_USB_OTG_FS
#error  "USE_USB_OTG_HS or USE_USB_OTG_FS should be defined"
#endif
#else 
#ifndef USE_ULPI_PHY
#ifndef USE_EMBEDDED_PHY
#error  "USE_ULPI_PHY or USE_EMBEDDED_PHY should be defined"
#endif
#endif
#endif
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined   (__GNUC__)        
#define __ALIGN_END    __attribute__ ((aligned (4)))
#define __ALIGN_BEGIN
#else
#define __ALIGN_END
#if defined   (__CC_ARM)      
#define __ALIGN_BEGIN    __align(4)
#elif defined (__ICCARM__)    
#define __ALIGN_BEGIN
#endif 
#endif 
#else
#define __ALIGN_BEGIN
#define __ALIGN_END
#endif 
#if defined (__CC_ARM)         
#define __packed    __packed
#elif defined (__ICCARM__)     
#define __packed    __packed
#elif defined   ( __GNUC__ )   
#define __packed    __attribute__((__packed__))
#endif 

#endif 

