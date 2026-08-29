#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zpin.h"
#include "zuart4.h"
#include "zcli.h"
#include "ztask.h"
#include "zi2c.h"
#include "zkey.h"
#include "zevent.h"

// app
#include "cli.h"
#include "tasks.h"
#include "disp.h"
#include "events.h"
#include "key.h"
#include "meas.h"

// bsp
#include "usart1_hal.h"
//#include "spi1_hal.h"
#include "adc_hal.h"
#include "timer3_hal.h"

// drv
#include "ssd1306_i2c.h"

extern class ZUART4 u1;
extern class ZCli cli;
extern class ZTASK zt;
extern class ZKey zk;
extern class ZEVENT ze;
extern class MEAS meas;

#endif
