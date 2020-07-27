#include "platform.h"

static void hello(void)
{
    printf("Hello, world.\n");
}

static void led(void)
{
    GPIOC->ODR ^= GPIO_Pin_13;
}

void TASKS_Poll(void)
{
    zt_poll();
}

static void cubic(void)
{
    static int count = 0;
    static int mode = 0;
    unsigned char test[64];
    if(mode == 0) {
        for(int i = 0; i < 64; i++) {
            test[i] = 1 << (count % 8);
        }
    }
    else {
        for(int i = 0; i < 64; i++) {
            if((i + count) % 8 == 0)
                test[i] = 0xff;
            else
                test[i] = 0x00;
        }
    }
    HC595_Write(test, 64);
    count++;
    if(count % 8 == 0)
        mode = !mode;
}

static void pset(unsigned char* buf, int x, int y)
{
    buf[x] = y;
}

static void draw_line(unsigned char* buf, int x1, int y1, int x2, int y2)
{
    int n, dx, dy, sgndx, sgndy, dxabs, dyabs, x, y, drawx, drawy;

    dx = x2 - x1;
    dy = y2 - y1;
    dxabs = (dx > 0) ? dx : -dx;
    dyabs = (dy > 0) ? dy : -dy;
    sgndx = (dx > 0) ? 1 : -1;
    sgndy = (dy > 0) ? 1 : -1;
    x = dyabs >> 1;
    y = dxabs >> 1;
    drawx = x1;
    drawy = y1;

    pset(buf, drawx, drawy);

    if(dxabs >= dyabs) {
        for(n = 0; n < dxabs; n++) {
            y += dyabs;
            if(y >= dxabs) {
                y -= dxabs;
                drawy += sgndy;
            }
            drawx += sgndx;
            pset(buf, drawx, drawy);
        }
    }
    else {
        for(n = 0; n < dyabs; n++) {
            x += dxabs;
            if(x >= dyabs) {
                x -= dyabs;
                drawx += sgndx;
            }
            drawy += sgndy;
            pset(buf, drawx, drawy);
        }
    }
}

#if 0
static void cubic2(void)
{
    static int count = 0;
    unsigned char buf[64];
    int terminals[][] = {    //
        {0, 0, 7, 7},    //
            {0, 1, 7, 6},    //
            {0, 2, 7, 5},    //
            {0, 3, 7, 4},    //
            {0, 4, 7, 3},    //
            {0, 5, 7, 2},    //
            {0, 6, 7, 1},    //
            {0, 7, 7, 0},    //
            {1, 7, 6, 0},    //
            {2, 7, 5, 0},    //
            {3, 7, 4, 0},    //
            {4, 7, 3, 0},    //
            {5, 7, 2, 0},    //
            {6, 7, 1, 0},    //
            {7, 7, 0, 0},    //
            {7, 6, 0, 1},    //
            {7, 5, 0, 2},    //
            {7, 4, 0, 3},    //
            {7, 3, 0, 4},    //
            {7, 2, 0, 5},    //
            {7, 1, 0, 6},    //
            {7, 0, 0, 7},    //
            {6, 0, 1, 7},    //
            {5, 0, 2, 7},    //
            {4, 0, 3, 7},    //
            {3, 0, 4, 7},    //
            {2, 0, 5, 7},    //
            {1, 0, 6, 7}    //
        };
    draw_line(buf, terminals[count][0], terminals[count][1],
        terminals[count][2], terminals[count][3]);

    HC595_Write(buf, 64);
    count++;
    if(count >= sizeof(terminals) / sizeof(terminals[0]))
        count = 0;
}
#endif

void TASKS_Config(void)
{
    zt_bind(hello, 250, 0);
    zt_bind(led, 1000, 1);
    zt_bind(zk_poll, 1, 1);
    zt_bind(cubic, 100, 1);
}
