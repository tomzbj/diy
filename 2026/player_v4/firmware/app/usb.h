#ifndef _USB_H
#define _USB_H

#include "misc.h"
#include "platform.h"

extern "C" void USB_Config(void);
extern "C" void mtp_upload_poll(void);
extern "C" void mtp_host_disconnected(void);
extern "C" void USB_Write(uint8_t* data, int size);
extern "C" void USB_Read(uint8_t* data, int size);
extern "C" void uprintf(const char* fmt, ...);

#endif
