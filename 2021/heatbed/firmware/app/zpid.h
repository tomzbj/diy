#ifndef _ZPID_H
#define _ZPID_H

typedef struct {
    float kp, ki, kd;
    float lim_sum_err, sum_err, prev_err, inteval;
} zpid_t;

float zpid(zpid_t* p, float sp, float pv);

#endif
