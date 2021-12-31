#ifndef _DATACONFIG_H
#define _DATACONFIG_H

typedef struct {
    struct {
        int reserved;
    } data;
    struct {
        int att;
    } cfg;
    struct {
        int reserved;
    } status;
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);

#endif
