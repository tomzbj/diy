#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "cocoos.h"

// thirdparty
#include "zuart2.h"
#include "zpin.h"
#include "zcli.h"
#include "flasheep.h"
#include "zi2c.h"
#include "zcrc32.h"

// bsp
#include "usart_hal.h"
#include "adc_hal.h"
#include "rtc_hal.h"

// drv
#include "nrf24l01.h"
#include "wht20.h"

// app
#include "board.h"
#include "tasks.h"
#include "dataconfig.h"
#include "cli.h"
#include "rf.h"
#include "sht.h"
#include "nrf24l01_ble.h"
#include "bds.h"

//class board;
extern class ZUart2 u1, u2;
extern class DataConfig dc;
extern class ZCli cli;
extern class ZKey zk;
extern class WHT20 sensor;
extern class CRTC rtc;
extern class CADC adc;
extern class BDS bds;

#endif
