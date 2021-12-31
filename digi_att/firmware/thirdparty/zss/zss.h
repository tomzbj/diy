#ifndef _ZSS_H
#define _ZSS_H

typedef struct {
    void (*set_segment_f)(unsigned int segment, int state);
    void (*set_digit_f)(unsigned int digit, int state);
    int digits, segments;
} zss_cfg_t;

void zss_update(void);
void zss_init(zss_cfg_t* cfg);
void zss_set_num(int num);
void zss_set_dot(int pos);
void zss_set_raw(unsigned char* raw);

#endif
