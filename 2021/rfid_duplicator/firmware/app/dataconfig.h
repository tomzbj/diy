#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#include "misc.h"

typedef struct {
    struct {
    } cfg;
    struct {
        unsigned char buffer[1024];
        int card_id;
        int buffer_id;
    } data;
    struct {
    } state;
} DataConfig_t;

DataConfig_t* DC_Get(void);
void DC_Update(void);

#endif
