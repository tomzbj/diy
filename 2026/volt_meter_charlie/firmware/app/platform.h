#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zpin.h"
#include "zuart4.h"
#include "zcli.h"
#include "ztask.h"
#include "zcharlie.h"

// app
#include "cli.h"
#include "tasks.h"
#include "disp.h"

// bsp
#include "usart_hal.h"
#include "adc_hal.h"

// drv

extern class ZUART4 u1;
extern class ZCli cli;
extern class ZTASK zt;
extern class ZCHARLIE charlie;

#endif
