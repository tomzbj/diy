#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

#include "usb_conf.h"

#define USBD_CFG_MAX_NUM                1
#define USBD_ITF_MAX_NUM                1
#define USBD_SELF_POWERED               
#define USB_MAX_STR_DESC_SIZ            255 
#define CDC_IN_EP                       0x81  
#define CDC_OUT_EP                      0x03  
#define CDC_CMD_EP                      0x82  
#define CDC_DATA_MAX_PACKET_SIZE       64   
#define CDC_CMD_PACKET_SZE             8    
#define CDC_IN_FRAME_INTERVAL          5    
#define APP_RX_DATA_SIZE               2048 
#define APP_FOPS                        VCP_fops

#endif 

