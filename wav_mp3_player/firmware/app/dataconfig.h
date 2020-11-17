#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#define DC_IN_BUF_SIZE  4096
#define DC_OUT_BUF_SIZE 9216

typedef struct {
    struct {
        unsigned char in_buf[DC_IN_BUF_SIZE];
        unsigned char out_buf[DC_OUT_BUF_SIZE];
    } data;
    struct {
        int reserved;
    } cfg;
    struct {
        int reserved;
    } status;
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);

#endif
