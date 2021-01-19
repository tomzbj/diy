#include "platform.h"
#include <stdlib.h>

#define BUF_SIZE 32

static void hello(void)
{
    xprintf("Hello, world.\n");
}

int cmp_f(const void* pa, const void* pb)
{
    float a = *(float*)pa;
    float b = *(float*)pb;
    return (a - b);
}

static void lm75(void)
{
    static float buf[BUF_SIZE];
    float buf2[BUF_SIZE];
    static int count = 0;
    DataConfig_t* pdc = DC_Get();
    buf[count] = pdc->data.t_pv = LM75_ReadTemp() / 256.0;
    count++;
    count %= BUF_SIZE;
    for(int i = 0; i < BUF_SIZE; i++)
        buf2[i] = buf[i];
    qsort(buf2, BUF_SIZE, sizeof(buf2[0]), cmp_f);
    float sum = 0;
    for(int i = BUF_SIZE / 4; i < BUF_SIZE * 3 / 4; i++) {
        sum += buf2[i];
    }
    pdc->data.t_pv_disp = sum / (BUF_SIZE / 2);
}

static void pid(void)
{
    DataConfig_t* pdc = DC_Get();

    static zpid_t pid;
    pid.kp = pdc->cfg.p;
    pid.ki = pdc->cfg.i;
    pid.kd = pdc->cfg.d;
    pid.inteval = 0.1; // in s
    pid.lim_sum_err = 500;
//    pid.lim_err = 20.0;
//    pid.prev_err = 0;
//    pid.sum_err = 0;

//    static float prev_t;

    int pwm = (int)zpid(&pid, pdc->status.t_sp, pdc->data.t_pv);
//        prev_t * 0.9 + pdc->data.t_pv * 0.1); prev_t = pdc->data.t_pv;

    if(pwm < 1)
        pwm = 1;
    if(pwm > 500)
        pwm = 500;
    pdc->status.pwm = pwm;
    TIM_SetCompare1(TIM15, pdc->status.pwm);
}

void led(void)
{
    DataConfig_t* pdc = DC_Get();
    float n = 1UL << (int)(pdc->status.pwm / 100);
//    n *= 30;
    if(n > 999)
        n = 999;
    TIM_SetCompare1(TIM16, (int)n);
}

void TASKS_Poll(void)
{
    zt_poll();
}

void TASKS_Config(void)
{
    zt_bind(hello, 500, 0);
    zt_bind(lm75, 3, 1);
    zt_bind(pid, 100, 1);
    zt_bind(led, 100, 1);
    zt_bind(zk_poll, 1, 1);
    zt_bind(DISP_Update, 1000, 1);
}
