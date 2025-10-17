#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zcli.h"
#include "zpin.h"
#include "zuart3.h"
#include "cocoos.h"
#include "zevent.h"
#include "zkey.h"

// bsp
#include "usart_gdf4.h"
#include "timer_gdf4.h"
#include "dac_gdf4.h"
#include "adc_gdf4.h"

// drv

// app
#include "cli.h"
#include "tasks.h"
#include "rf.h"
#include "disp.h"
#include "events.h"
#include "key.h"

extern class ZUart3 u0;
extern class ZCli cli, cli2;
extern class ZEVENT ze;
extern class ZKey zk;
extern class EVENTS events;

#endif
