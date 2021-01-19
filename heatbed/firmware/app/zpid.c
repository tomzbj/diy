#include "zpid.h"
#include "platform.h"

#define abs(x) ((x)>0?(x):(-(x)))

float zpid(zpid_t* p, float sp, float pv)
{
    float err = sp - pv;
//    float output;

//    if(abs(err) < p->lim_err)         //
    if((abs(p->sum_err * p->ki) > p->lim_sum_err)
        && (p->sum_err * err > 0)) {
        // saturated, do nothing
    }
    else {
        p->sum_err += err * p->inteval;
    }
//    else        p->sum_err = 0;
    float d = (err - p->prev_err) / p->inteval;
    p->prev_err = err;

    {
        DataConfig_t* pdc = DC_Get();
        if(pdc->status.debug) {
            static int count;
            count++;
            count %= 10;
            if(count == 0)
                xprintf("p part: %.1f     i part: %.1f     d part: %.1f\n",
                    p->kp * err, p->ki * p->sum_err, p->kd * d);
        }
    }

    return p->kp * err + p->ki * p->sum_err + p->kd * d;
}

float zpid2(zpid_t* p, float sp, float pv)
{
    float err = sp - pv;

    p->sum_err += err * p->inteval;
    float d = (err - p->prev_err) / p->inteval;
    p->prev_err = err;

    {
        DataConfig_t* pdc = DC_Get();
        if(pdc->status.debug) {
            static int count;
            count++;
            count %= 10;
            if(count == 0)
                xprintf("p part: %.1f     i part: %.1f     d part: %.1f\n",
                    p->kp * err, p->ki * p->sum_err, p->kd * d);
        }
    }

    return p->kp * err + p->ki * p->sum_err + p->kd * d;
}
