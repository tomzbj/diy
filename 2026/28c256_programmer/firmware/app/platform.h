#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zpin.h"
#include "zuart4.h"
#include "zcli.h"
#include "ztask.h"
#include "flasher_at28c.h"

// app
#include "cli.h"
#include "tasks.h"
#include "usb.h"
#include "eeprom.h"

// bsp
#include "usart1_hal.h"

// drv
#include "at28cxx.h"

extern class ZUART4 u1, ucdc;
extern class ZCli cli, cli_cdc;
extern class ZTASK zt;
extern class AT28CXX at28cxx;
extern class FLASHER_AT28C flasher, flasher_u;

#endif
