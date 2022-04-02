#ifndef _XPT2046_H
#define _XPT2046_H

#define XPT2046_FILTER_SIZE 16
#define XPT2046_FILTER_DIFF 5

typedef struct {
    unsigned char (*readwrite_f)(unsigned char);
    void (*cs_f)(int state);
} xpt2046_cfg_t;

typedef struct {
    unsigned short x, y;
} xpt2046_coord_t;

void XPT2046_Init(xpt2046_cfg_t* cfg);

xpt2046_coord_t XPT2046_Read(void);
int XPT2046_ReadX(void);
int XPT2046_ReadY(void);
unsigned short XPT2046_ReadVbat(void);

#endif
