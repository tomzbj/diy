#ifndef _SHTC3_H
#define _SHTC3_H

#include "zi2c.h"
// i2c addr: 0x70

typedef struct {
    unsigned short RH, T;
} shtc3_result_t;

class shtc3 {
public:
    shtc3(zi2c& i2c);
    shtc3_result_t read(void);
private:
    zi2c& _i2c;
};

#endif
