#include "ztask.h"

typedef struct {
    zt_func_t func;
    unsigned long timeout, repeat;
    int en;
} zt_task_t;

static struct {
    unsigned long ticks;
    int num_tasks;
    zt_task_t tasks[ZT_MAX_TASKS];
} g;

void zt_tick(void)
{
    // overflow not handled. for 1ms tick, it could run 2^32 * 1ms = ~50 days.
    g.ticks++;
}

void zt_poll(void)
{
    int i;
    for(i = 0; i < g.num_tasks; i++) {
        if(g.ticks >= g.tasks[i].timeout) {
            g.tasks[i].timeout = g.ticks + g.tasks[i].repeat;
            if(g.tasks[i].en)
                g.tasks[i].func();
        }
    }
}

void zt_stop(int id)
{
    if(id < ZT_MAX_TASKS)
        g.tasks[id].en = 0;
}

void zt_start(int id)
{
    if(id < ZT_MAX_TASKS)
        g.tasks[id].en = 1;
}

int zt_bind(zt_func_t func, int repeat, int en)
{
    if(g.num_tasks < ZT_MAX_TASKS) {
        g.tasks[g.num_tasks].func = func;
        g.tasks[g.num_tasks].repeat = repeat;
        g.tasks[g.num_tasks].timeout = 0;
        g.tasks[g.num_tasks].en = en;
        return g.num_tasks++;
    }
    else
        return -1;
}
