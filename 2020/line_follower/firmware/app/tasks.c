#include "platform.h"
#include <math.h>

#define NUM_CHANNELS 6

static void hello(void)
{
//    printf("Hello, world.\n");
    DataConfig_t* pdc = DC_Get();
    int min = 99999;
    int n = -1;
    for(int i = 0; i < NUM_CHANNELS; i++) {
        printf("%8lu  ", pdc->data.ch[i]);
        if(pdc->data.ch[i] < min) {
            min = pdc->data.ch[i];
            n = i;
        }
    }
    printf("%d  \n", n);
}

typedef struct {
    float p, i, d;
    float lim_err, sum_err, prev_err;
} zpid_t;

static float do_pid(zpid_t* p, float sp, float pv)
{
    float err = sp - pv;
    float output;

    if(fabs(err) < p->lim_err)         //
        p->sum_err += err;
    else
        p->sum_err = 0;

    output = (p->p * err + p->i * p->sum_err + p->d * (err - p->prev_err));
    p->prev_err = err;

    return output;
}

static void pid(void)
{
    DataConfig_t* pdc = DC_Get();

    static zpid_t pid;
    pid.p = pdc->cfg.p;
    pid.p = pdc->cfg.i;
    pid.p = pdc->cfg.d;
    pid.lim_err = 100;
    pid.prev_err = 0;
    pid.sum_err = 0;

    int pwm = (int)do_pid(&pid, 3, pdc->data.pos);
    if(pwm < 0)
        pwm -= 30;
    else if(pwm > 0)
        pwm += 30;
    {
        static int count;
        count++;
        count %= 100;
        if(count == 0)
            printf("%d %f\n", pwm, pdc->data.pos);
    }
    MOTOR_Set(MOTOR_L, -100 - pwm);
    MOTOR_Set(MOTOR_R, -100 + pwm);
}

void TASKS_Poll(void)
{
    zt_poll();
}

void TASKS_Config(void)
{
    zt_bind(hello, 250, 0);
    zt_bind(pid, 10, 1);
}
