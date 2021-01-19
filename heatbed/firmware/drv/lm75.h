#ifndef _LM75_I2C_H
#define _LM75_I2C_H

#include "zi2c.h"

void LM75_Init(zi2c_t *i2c);
signed short LM75_ReadTemp(void);

#endif
