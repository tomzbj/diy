#ifndef _DATACONFIG_H
#define _DATACONFIG_H

#include "platform.h"

enum {
    DC_STATE_STOPPED, DC_STATE_RUNNING
};

typedef struct {
    struct {
        int time;
    } data;
    struct {
    } cfg;
    struct {
        int state;
    } status;
    ztar_obj_t tar;
} DataConfig_t;

void DC_Config(void);
void DC_Update(void);
DataConfig_t* DC_Get(void);

#endif
