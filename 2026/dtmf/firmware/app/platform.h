#ifndef _PLATFORM_H
#define _PLATFORM_H

// conf
#include "stm32f1xx_hal_conf.h"

// thirdparty
#include "zuart3.h"
#include "zpin.h"
#include "zcli.h"
#include "ztask.h"
#include "zdds.h"

// bsp
#include "usart1_hal.h"
#include "dac_hal.h"
#include "adc_hal.h"
#include "timer3_hal.h"
#include "timer6_hal.h"

// drv

// app
#include "cli.h"
#include "tasks.h"
#include "dds.h"
#include "dtmf.h"

//class board;
extern class ZUart3 u1;
extern class ZCli cli;
extern class ZTASK zt;
extern class ZDDS dds;
extern class DTMF_MOD mod;    //(dds, 300, 50);
extern class DTMF_DEM dem;    //(dds, 300, 50);

#endif
