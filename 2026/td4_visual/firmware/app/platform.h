#ifndef _PLATFORM_H
#define _PLATFORM_H

// conf
#include "stm32f0xx_hal_conf.h"

// thirdparty
#include "zuart4.h"
#include "zpin.h"
#include "zcli.h"
#include "ztask.h"
#include "zkey.h"

// bsp
#include "usart1_hal.h"
#include "spi1_hal.h"

// app
#include "cli.h"
#include "tasks.h"
#include "disp.h"
#include "rom.h"
#include "key.h"
#include "eeprom.h"

// drv
#include "st7789.h"

//class board;
extern class ZUART4 u1;
extern class ZCli cli;
extern class ZTASK zt;
extern class TFT_ST7789 tft;
extern class ROM rom;
extern class ZKey zk;
extern class DISP disp;

#endif
