#include <math.h>
#include "usart.h"

void dopid(void)
{
}

void PID_Adjust(float volt_set, float volt_now)
{
    const float KP = 90, KI = 2;    //, KD = 0;
    static int volt_ctrl = 100;
    static float err = 0, sum_err = 0;  //, err1 = 0;//, err2 = 0; //本次、上次、上上次误差

    err = volt_set - volt_now;

    if(fabs(err) < 999) {        // 误差小于0.5度时启用I
        sum_err += err;
    }
    else {
        sum_err = 0;
    }
    volt_ctrl = (int)(KP * err + KI * sum_err);
    if(volt_ctrl > 470)
        volt_ctrl = 470; //限幅
    if(volt_ctrl < 0)
        volt_ctrl = 0;
//    CJR_SetDuty(volt_ctrl);
    TIM_SetCompare1(TIM14, volt_ctrl);

    /*
    static int count = 0;
    count++;
    if(count >= 1000) {
        count = 0;
        uprintf(USART1, "### %8.1f %8.4f %8.1f %8.3f %8.3f %8.3f %8d ###\n", KP, err, KI, sum_err, volt_set, volt_now, volt_ctrl);
    }
    */
}
