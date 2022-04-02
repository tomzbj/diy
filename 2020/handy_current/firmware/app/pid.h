#ifndef _PID_H_
#define _PID_H_

#define PID_TYPE_FINE 10001
#define PID_TYPE_COARSE 10002

void PID_Adjust(float curset, float curnow);
void PID_SetType(int type);
void PID_SetDebug(int debug);

#endif /* _PID_H_ */
