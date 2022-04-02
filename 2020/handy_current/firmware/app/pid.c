#include <math.h>
#include "misc.h"
#include "pid.h"
#include "usart_f0.h"

static struct {
    int type;
    bool debug;
} g;

void PID_SetDebug(int debug)
{
    if(debug)
        g.debug = true;
    else
        g.debug = false;
}

void PID_SetType(int type)
{
    g.type = type;
}

void PID_Coarse(float curset, float curnow)
{
    const int KP = 3000, KI = 10;    //, KD = 0;
    static int pwm = 1500;
    static float err = 0, err1 = 0;    //, err1 = 0;//, err2 = 0; //本次、上次、上上次误差
    static float sum_err = 0;
    static int pid_count = 0;

    err = curset - curnow;

    if(fabs(err) < 0.000005 && fabs(err1) < 0.000005
        && fabs(err - err1) < 0.000001) {
        g.type = PID_TYPE_FINE;
        sum_err = 0;
        return;
    }
    sum_err += err;
    pwm = (int)(KP * err + KI * sum_err);
    if(pwm > 2700)
        pwm = 2700; //限幅
    else if(pwm < 0)
        pwm = 0;
    if(pwm == 0 || pwm == 2700) {
        pid_count++;
        if(pid_count > 10000) {
            pid_count = 0;
            sum_err = 0;
        }
    }
    else
        pid_count = 0;
    if(g.debug) {
        static int count = 0;
        count++;
        count %= 100;
        if(count == 0)
            uprintf(USART3, "COARSE: %f %f %f %f %d\n", curset, curnow, err, sum_err, pwm);
    }
    DAC_SetDualChannelData(DAC_Align_12b_R, 2048, pwm);
    err1 = err;
}

void PID_Fine(float curset, float curnow)
{
    const int KP = 1500000, KI = 500;    //, KD = 0;
    static int pwm = 0;
    static float err = 0;    //, err1 = 0;//, err2 = 0; //本次、上次、上上次误差
    static float sum_err = 0;
    static int fine_count = 0;

    err = curset - curnow;
    sum_err += err;
    pwm = (int)(KP * err + KI * sum_err);
    if(pwm > 2000) {
        pwm = 2000; //限幅
        fine_count++;
    }
    else if(pwm < -2000) {
        pwm = -2000;
        fine_count++;
    }
    else
        fine_count = 0;
    if(fine_count > 1000) {
        fine_count = 0;
        sum_err = 0;
        g.type = PID_TYPE_COARSE;
        return;
    }
    if(g.debug) {
        static int count = 0;
        count++;
        count %= 100;
        if(count == 0)
            uprintf(USART3, "FINE: %f %f %f %f %d\n", curset, curnow, err,
                sum_err, pwm);
    }
    DAC_SetChannel2Data(DAC_Align_12b_R, pwm + 2048);
}

void PID_Adjust(float curset, float curnow)
{
    if(g.type == PID_TYPE_FINE)
        PID_Fine(curset, curnow);
    else
        PID_Coarse(curset, curnow);
}
