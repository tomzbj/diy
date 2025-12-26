#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zpin.h"
#include "zuart3.h"
#include "zcli.h"
#include "ztask.h"
#include "zpid.h"

// app
#include "cli.h"
#include "tasks.h"
#include "counter.h"
#include "sensor.h"
#include "tempctl.h"

// bsp
#include "usart1_hal.h"
#include "usart2_hal.h"
#include "adc_hal.h"
#include "timer1_hal.h"
#include "timer14_hal.h"
#include "timer16_hal.h"

// drv
#include "lm75.h"

extern class ZUart3 u1, u2;
extern class ZCli cli;
extern class ZTASK zt;
extern class COUNTER counter;
extern class LM75 lm75;
extern class ZPID zpid;

#endif
