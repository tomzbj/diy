#ifndef _PLATFORM_H
#define _PLATFORM_H

// conf
#include "stm32f4xx_hal_conf.h"

// thirdparty
#include "zuart4.h"
#include "zpin.h"
#include "zcli.h"
#include "ztask.h"
#include "zevent.h"

// bsp
#include "usart1_hal.h"
#include "sd_hal.h"
#include "i2s_hal.h"

// app
#include "cli.h"
#include "tasks.h"
#include "fs.h"
#include "fs_lock.h"
#include "flash.h"

// audio
#include "player.h"
#include "audio.h"

//class board;
extern class ZUART4 u1;
extern class ZCli cli;
extern class ZTASK zt;
extern class ZEVENT ze;
extern class PLAYER player;

#endif
