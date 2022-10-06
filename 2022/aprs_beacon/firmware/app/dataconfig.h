#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#include "misc.h"

typedef struct {
    struct {
    } data;
    struct {
    } cfg;
    struct {
        int debug;
    } status;
} DataConfig_t;

DataConfig_t* DC_Get(void);
void DC_Update(void);
void DC_SetDefaults(void);
void DC_Config(void);

#endif
