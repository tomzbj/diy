#ifndef _TIMER_H
#define _TIMER_H

#include "misc.h"

typedef void (*timer_func_t)(void);

enum {
    TIMER_REG_SUCCESS, TIMER_REG_FAILED
};

void TIMER_Config(void);
void TIMER_Loop(void);
int TIMER_RegisterFunc(timer_func_t func, int timeout);
void TIMER_Start(int id);
void TIMER_Stop(int id);

#endif
