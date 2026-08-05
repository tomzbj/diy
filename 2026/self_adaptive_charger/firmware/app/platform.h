#ifndef _PLATFORM_H
#define _PLATFORM_H

// conf
#include "stm32f0xx_hal_conf.h"

// thirdparty
#include "zuart4.h"
#include "zpin.h"
#include "zcli.h"
#include "ztask.h"

// bsp
#include "usart1_hal.h"
#include "timer15_hal.h"
#include "adc_hal.h"

// app
#include "cli.h"
#include "tasks.h"
#include "disp.h"
#include "charger.h"

//class board;
extern class ZUART4 u1;
extern class ZCli cli;
extern class ZTASK zt;
extern class CHARGER charger;

#endif
