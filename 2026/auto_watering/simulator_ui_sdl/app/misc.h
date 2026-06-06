#ifndef _MISC_H
#define _MISC_H

// thirdparty
#include "zuart4.h"
#include "zcli.h"

// basic
#include <cstdio>
#include <cstring>
#include <cstdint>

// stl
#include <string>
#include <map>
#include <sstream>
#include <vector>

// ui
#include "ui.h"

// app
#include "sdl_wrapper.h"
#include "udp_server.h"
#include "cli.h"

// utilities
#define _dbg() do {printf("%d %s\n", __LINE__, __FILE__); fflush(stdout);} while(0)

#endif
