#ifndef _SI5351_H_
#define _SI5351_H_

#include "zi2c.h"

enum {
    SI5351_PLLA, SI5351_PLLB
};

enum {
    SI5351_MS0,
    SI5351_MS1,
    SI5351_MS2,
    SI5351_MS3,
    SI5351_MS4,
    SI5351_MS5,
    SI5351_MS6,
    SI5351_MS7
};

enum {
    SI5351_RDIV_DIV1 = 0,
    SI5351_RDIV_DIV2 = 1,
    SI5351_RDIV_DIV4 = 2,
    SI5351_RDIV_DIV8 = 3,
    SI5351_RDIV_DIV16 = 4,
    SI5351_RDIV_DIV32 = 5,
    SI5351_RDIV_DIV64 = 6,
    SI5351_RDIV_DIV128 = 7
};

enum {
    SI5351_OK = 0, SI5351_ERR = 1
};

enum {
    SI5351_OUTPUT_STATE_DISABLE, SI5351_OUTPUT_STATE_ENABLE
};
void SI5351_Init(zi2c_t* i2c);
int SI5351_SetPLL(int pll_id, int a, int b, int c);
int SI5351_SetMSParams(int multisynth_id, int pll_id, int a, int b, int c);
int SI5351_SetOutputState(int clk_id, int state);
int SI5351_SetRDiv(int rdiv_id, int r);

int SI5351_SetFreq(int freq);    //int ms_id, int pll_id, int a, int b, int c)
#endif /* SI5351_H_ */
