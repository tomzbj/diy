#ifndef _PLATFORM_H
#define _PLATFORM_H

// thirdparty
#include "zuart.h"
#include "board.h"
#include "ztask.h"
#include "zpin.h"
#include "zi2c.h"
#include "flash_eeprom.h"

// bsp
#include "gpio_gdf3.h"
#include "usart_gdf3.h"
#include "spi_gdf3.h"

// drv
#include "nrf24l01.h"
#include "shtc3.h"

// app
#include "tasks.h"
#include "cli.h"
#include "dataconfig.h"
#include "rf.h"
#include "sht.h"

//class board;
extern class board b;
extern class zuart u0;
extern class ztask zt;
extern class data_config dc;

#endif
