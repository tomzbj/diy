#ifndef _PLATFORM_H
#define _PLATFORM_H

// conf
#include "arduino.h"
#include "misc.h"
#include "config.h"

// thirdparty
#include "zcli.h"
#include "ztask.h"

// bsp
#include "udp_server.h"
#include "net.h"

// app
#include "tasks.h"
#include "cli.h"
#include "sht.h"
#include "disp.h"
#include "rtc.h"

// drv
#include "wht20.h"

//class
extern class ZTASK zt;
extern class ZCli zcli, zcli_udp;
extern class UDP_SERVER udp;
extern class WHT20 sensor;
extern class DISP disp;

#endif
