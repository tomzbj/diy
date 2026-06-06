#ifndef _MOTOR_H
#define _MOTOR_H

enum {
  MOTOR_STOP, MOTOR_RUN
};
void MOTOR_Config(void);
void MOTOR_Set(int state);

#endif
