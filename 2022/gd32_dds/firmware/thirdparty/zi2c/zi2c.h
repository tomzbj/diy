#ifndef _SOFTI2C_H
#define _SOFTI2C_H

#include "misc.h"

typedef struct zi2cobj {
    // low-level ops, implemented by user
    void (*setsda_f)(int state);
    void (*setscl_f)(int state);
    int (*getsda_f)(void);

    // mid-level ops
    void (*start_f)(struct zi2cobj* obj);
    void (*stop_f)(struct zi2cobj* obj);
    unsigned char (*readbyte_ack_f)(struct zi2cobj* obj);
    unsigned char (*readbyte_nack_f)(struct zi2cobj* obj);
    void (*writebyte_f)(struct zi2cobj* obj, unsigned char);

    // high-level ops
    void (*write_reg_f)(struct zi2cobj*, unsigned char reg, unsigned char val);
    unsigned char (*read_reg_f)(struct zi2cobj*, unsigned char reg);
    void (*write_regs_f)(struct zi2cobj*, unsigned char reg, const unsigned char* data, int count);
    void (*read_regs_f)(struct zi2cobj*, unsigned char reg, unsigned char* data, int count);

    // other params
    unsigned char addr;
    int retry_count;
} zi2c_t;

void zi2c_init(zi2c_t* zi2c);

#endif
