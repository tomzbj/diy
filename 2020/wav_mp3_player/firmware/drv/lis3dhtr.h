#ifndef _LIS3DHTR_I2C_H
#define _LIS3DHTR_I2C_H

#include "misc.h"
#include "zi2c.h"

typedef struct {
    signed short x, y, z;
} lis3_acclr_t;

void LIS3DHTR_Init(zi2c_t *i2c);
//void LIS3DHTR_Write(unsigned char* buf, int size);
unsigned char LIS3DHTR_ReadState(void);
lis3_acclr_t LIS3DHTR_ReadAcclr(void);

#endif
