#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#define DC_NUM_OF_SAMPLES 4000
typedef struct {
    struct {
        unsigned long adc_buf[DC_NUM_OF_SAMPLES];
    } data;
    struct {
    } cfg;
    struct {
    } status;
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);

#endif /* _DC_H */
