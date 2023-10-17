#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zuart.h"
#include "board.h"
#include "ztask.h"
#include "zpin.h"
#include "zcli.h"
#include "flasheep.h"

// bsp
#include "gpio_gdf3.h"
#include "usart_gdf3.h"

// drv

// app
#include "tasks.h"
#include "dataconfig.h"
#include "zgate.h"
#include "gates.h"
#include "cli.h"

//class board;
extern class Board b;
extern class ZUart u0;
extern class ZTask zt;
extern class DataConfig dc;
extern class ZCli cli;

#endif
