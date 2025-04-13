#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zcli.h"
#include "zpin.h"
#include "zuart3.h"
#include "ugui.h"
#include "cocoos.h"

// bsp
#include "usart_gdf4.h"
#include "timer_gdf4.h"

// drv

// app
#include "cli.h"
#include "charlie.h"
#include "disp.h"
#include "tasks.h"

extern class ZUart3 u0;
extern class ZCli cli, cli2;

#endif
