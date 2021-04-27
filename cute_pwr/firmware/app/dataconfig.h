#ifndef _DATACONFIG_H
#define _DATACONFIG_H

typedef struct {
    struct {
        float vout, iout, vbat;
    } data;
    struct {
    } cfg;
    struct {
        float v_dcdc, v_sp, i_sp;
    } status;
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);

#endif /* _DC_H */
