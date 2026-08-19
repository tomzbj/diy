#ifndef _PLATFORM_H
#define _PLATFORM_H

// conf
#include "stm32f1xx_hal_conf.h"

// thirdparty
#include "zuart4.h"
#include "zpin.h"
#include "zcli.h"
#include "ztask.h"

// bsp
#include "usart1_hal.h"
#include "spi1_hal.h"
#include "timer3_hal.h"
#include "timer5_hal.h"
#include "adc_hal.h"
#include "pwr_hal.h"

// drv

// app
#include "cli.h"
#include "tasks.h"
#include "disp.h"
#include "ui.h"
#include "meas.h"
#include "bat.h"

//class board;
extern class ZUART4 u1;
extern class ZCli cli;
extern class ZTASK zt;

#endif
