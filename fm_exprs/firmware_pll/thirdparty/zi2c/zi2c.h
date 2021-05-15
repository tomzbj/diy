#ifndef _ZI2C_H
#define _ZI2C_H

#include "misc.h"

typedef struct zi2cobj {
    void (*setsda_1_f)(void);
    void (*setsda_0_f)(void);
    void (*setscl_1_f)(void);
    void (*setscl_0_f)(void);
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
