#ifndef _MOTOR_H
#define _MOTOR_H

enum {
    MOTOR_L, MOTOR_R
};

void MOTOR_Config(void);
void MOTOR_Set(int motor_id, int speed);

#endif
