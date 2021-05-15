#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#include "misc.h"

typedef struct {
    struct {
    } cfg;
    struct {
        unsigned long freq;     // in kHz
    } data;
    struct {
    } state;
} DataConfig_t;

DataConfig_t* DC_Get(void);
void DC_Update(void);
void DC_Config(void);

#endif
