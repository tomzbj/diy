#include "misc.h"
#include "math.h"
#include "platform.h"

static void hello(void)
{
    printf("Hello, world.\n");
}

static void lis3(void)
{
    lis3_acclr_t a;
    a = LIS3DHTR_ReadAcclr();
    float m = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    printf("%.3f %.3f %.3f\n", a.x / m, a.y / m, a.z / m);
}

static void led(void)
{
    GPIOA->ODR ^= GPIO_Pin_0;
}

//static void i2s(void) { }

void TASKS_Poll(void)
{
    zt_poll();
}

void TASKS_Config(void)
{
    zt_bind(hello, 500, 0);
    zt_bind(lis3, 250, 0);
    zt_bind(led, 100, 1);
//    zt_bind(i2s, 10, 1);
}
