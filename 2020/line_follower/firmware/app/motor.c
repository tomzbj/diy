#include "misc.h"
#include "platform.h"
#include <math.h>

void MOTOR_Set(int motor_id, int speed)
{
    typeof(TIM_SetCompare1)* pSetCompFunc1 = NULL, * pSetCompFunc2 = NULL;

    switch(motor_id) {
        case MOTOR_L:
            pSetCompFunc1 = TIM_SetCompare1;
            pSetCompFunc2 = TIM_SetCompare2;
            break;
        case MOTOR_R:
            pSetCompFunc1 = TIM_SetCompare3;
            pSetCompFunc2 = TIM_SetCompare4;
            break;
    }
    if(speed < 0)
        swap(pSetCompFunc1, pSetCompFunc2);

    speed = fabs(speed);
    if(speed > 999)
        speed = 999;
    pSetCompFunc1(TIM4, speed);
    pSetCompFunc2(TIM4, 0);
}
