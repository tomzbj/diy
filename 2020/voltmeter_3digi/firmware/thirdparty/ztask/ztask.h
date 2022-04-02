#ifndef _ZTASK_H
#define _ZTASK_H

#define ZT_MAX_TASKS 4

typedef void (*zt_func_t)(void);

// should be called in main loop
void zt_poll(void);

// timeout: repeat inteval;   en: start immediately or not
int zt_bind(zt_func_t func, int repeat, int en);

// should be called in systick_irqhandler
void zt_tick(void);

void zt_start(int id);
void zt_stop(int id);

#endif
