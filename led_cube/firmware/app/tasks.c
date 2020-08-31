#include "platform.h"
#include <string.h>

static void hello(void)
{
    printf("Hello, world.\n");
}

static void cube_view2(void)
{
    static int opened = 0;
    static FIL f;
    static FILINFO inf;
    static unsigned int size, n;
    char* fname = "2a.bin";

    if(!opened) {
        f_stat(fname, &inf);
        f_open(&f, fname, FA_READ);
        size = inf.fsize;
        opened = 1;
    }

    if(size > 0) {
        unsigned char buf[64];
        f_read(&f, buf, sizeof(buf), &n);
        HC595_Write(buf, 64);
        printf(".\n");
        size -= n;
    }

    if(f_eof(&f)) {
        f_close(&f);
        opened = 0;
        if(strcmp(fname, "2a.bin") == 0)
            fname = "1a.bin";
        else
            fname = "2a.bin";
    }
}

void TASKS_Poll(void)
{
    zt_poll();
}

void TASKS_Config(void)
{
    zt_bind(hello, 250, 0);
    zt_bind(cube_view2, 16, 1);
}
