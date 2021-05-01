#ifndef _ZI2C_H
#define _ZI2C_H

#include "misc.h"

typedef struct zi2cobj {
    void (*setsda_f)(int state);
    void (*setscl_f)(int state);
    int (*getsda_f)(void);
    void (*start_f)(struct zi2cobj* obj);
    void (*stop_f)(struct zi2cobj* obj);
    unsigned char (*readbyte_ack_f)(struct zi2cobj* obj);
    unsigned char (*readbyte_nack_f)(struct zi2cobj* obj);
    void (*writebyte_f)(struct zi2cobj* obj, unsigned char);
    unsigned char addr;
} zi2c_t;

void zi2c_init(zi2c_t* zi2c);

#endif
