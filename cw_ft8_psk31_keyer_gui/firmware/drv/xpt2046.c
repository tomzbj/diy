#include "xpt2046.h"

static xpt2046_cfg_t g;
#include "misc.h"

void XPT2046_Init(xpt2046_cfg_t* cfg)
{
    g = *cfg;
}

unsigned short __read(unsigned char c)
{
    register unsigned short t;
    unsigned short a[XPT2046_FILTER_SIZE];

    g.cs_f(0);
    g.readwrite_f(c);
    for(int i = 0; i < XPT2046_FILTER_SIZE; i++) {
        t = g.readwrite_f(0x00);
        t <<= 8;
        t |= g.readwrite_f(c);
        a[i] = t >> 3;
    }
    g.cs_f(1);

    for(int i = 0; i < XPT2046_FILTER_SIZE - 1; i++) {
        for(int j = i + 1; j < XPT2046_FILTER_SIZE; j++) {
            if(a[i] < a[j]) {
                ( {  t=a[i];a[i]=a[j]; a[j]=t;});
            }
        }
    }
    t = 0;
    for(int i = XPT2046_FILTER_SIZE / 4; i < XPT2046_FILTER_SIZE * 3 / 4; i++) {
//        printf("#%d\t", a[i]);
        t += a[i];
    }
//    printf("##\n");
    return t / (XPT2046_FILTER_SIZE / 2);
}

xpt2046_coord_t XPT2046_Read(void)
{
    static xpt2046_coord_t c;
    unsigned short t1, t2;
    t1 = __read(0xd3);
    t2 = __read(0xd3);
    if(abs(t2 - t1) < XPT2046_FILTER_DIFF)
        c.x = (t1 + t2) / 2;
    t1 = __read(0x93);
    t2 = __read(0x93);
    if(abs(t2 - t1) < XPT2046_FILTER_DIFF)
        c.y = (t1 + t2) / 2;

    return c;
}

unsigned short XPT2046_ReadVbat(void)
{
    return __read(0xa7);
}

int XPT2046_ReadX(void)
{
    unsigned short t1, t2;
    t1 = __read(0xd3);
    t2 = __read(0xd3);
//    printf("#X# %d %d\n", t1, t2);
    if(abs(t2 - t1) < XPT2046_FILTER_DIFF) {
        return (t1 + t2) / 2;
    }
    else
        return -1;
}

int XPT2046_ReadY(void)
{
    unsigned short t1, t2;
    t1 = __read(0x93);
    t2 = __read(0x93);
//    printf("#Y# %d %d\n", t1, t2);
    if(abs(t2 - t1) < XPT2046_FILTER_DIFF) {
        return (t1 + t2) / 2;
    }
    else
        return -1;
}
