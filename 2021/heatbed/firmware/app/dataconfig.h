#ifndef _DATACONFIG_H
#define _DATACONFIG_H

typedef struct {
    struct {
        float t_pv;
        float t_pv_disp;
    } data;
    struct {
        float p, i, d;
    } cfg;
    struct {
        int pwm;
        float t_sp;
        float t_step;
        int debug;
    } status;
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);

#endif
