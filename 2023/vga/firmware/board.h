#ifndef _BOARD_H
#define _BOARD_H

#include "platform.h"

class Board {

public:
    void init(void);
    void run(void);
};

extern class ZUart u0;
extern class ZTask zt;
extern class Board b;
extern class DataConfig dc;

extern class ZCli cli;

#endif
