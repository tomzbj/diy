#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zuart4.h"
#include "zpin.h"
#include "zcli.h"
#include "ztask.h"
#include "zkey.h"

// drv
#include "tft.h"
#include "gc9307.h"

// bsp
#include "usart1_hal.h"
#include "dac_hal.h"
#include "timer6_hal.h"
#include "timer3_hal.h"
#include "timer2_hal.h"
#include "spi3_hal.h"
#include "adc_hal.h"
#include "rtc_hal.h"
#include "iwdg_hal.h"

// app
#include "cli.h"
#include "disp.h"
#include "tasks.h"
#include "key.h"
#include "eeprom.h"
#include "motor.h"
#include "moisture.h"

// ui
#include "ui.h"

extern class ZUART4 u1;
extern class ZCli cli;
extern class ZTASK zt;
extern class ZKey zk;
extern uint32_t now;

#endif
