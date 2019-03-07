#include <stdio.h>
#include "misc.h"
#include "timer.h"
#include "batt.h"

#define MAX_TASKS 4
int timer_viewvolt;

typedef struct {
    timer_func_t func;
    int timeout;
    int repeat;
    bool en;
} task_t;

static struct {
    unsigned long ticks;
    int curr_num_of_tasks;
    task_t tasks[MAX_TASKS];
} g;

void TIMER_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 23999;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM7, ENABLE);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    g.ticks = 0;
    g.curr_num_of_tasks = 0;
    timer_viewvolt = TIMER_RegisterFunc(BATT_ViewVoltage, 1000);
//    TIMER_Start(timer_viewvolt);
}

void TIM7_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        g.ticks++;
    }
}

void TIMER_Loop(void)
{
    for(int i = 0; i < g.curr_num_of_tasks; i++) {
        if(g.ticks >= g.tasks[i].timeout) {
            g.tasks[i].timeout = g.ticks + g.tasks[i].repeat;
            if(g.tasks[i].en)
                g.tasks[i].func();
        }
    }
}

void TIMER_Stop(int id)
{
    if(id < MAX_TASKS)
        g.tasks[id].en = false;
}

void TIMER_Start(int id)
{
    if(id < MAX_TASKS)
        g.tasks[id].en = true;
}

int TIMER_RegisterFunc(timer_func_t func, int repeat)
{
    if(g.curr_num_of_tasks < MAX_TASKS) {
        g.tasks[g.curr_num_of_tasks].func = func;
        g.tasks[g.curr_num_of_tasks].repeat = repeat;
        g.tasks[g.curr_num_of_tasks].timeout = repeat;
        g.tasks[g.curr_num_of_tasks].en = false;
        return g.curr_num_of_tasks++;
    }
    else
        return -1;
}
