#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zpin.h"
#include "zkey.h"
#include "zevent.h"
#include "zcli.h"
#include "zuart3.h"

// app
#include "key.h"
#include "events.h"
#include "cli.h"
#include "fan.h"

// bsp
#include "usart_hal.h"
#include "timer_hal.h"
#include "lptim_hal.h"

// drv

extern class ZKey zk;
extern class ZEVENT ze;
extern class EVENTS events;
extern class ZUart3 u1;
extern class ZCli cli;

#endif
