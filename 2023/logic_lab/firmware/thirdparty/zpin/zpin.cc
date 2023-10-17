#include "zpin.h"

static const unsigned long gpio_port[] = {
#if defined(GPIOA)
GPIOA,
#else
    0,
#endif
#if defined(GPIOB)
GPIOB,
#else
    0,
#endif
#if defined(GPIOC)
GPIOC,
#else
    0,
#endif
#if defined(GPIOD)
GPIOD,
#else
    0,
#endif
#if defined(GPIOE)
GPIOE,
#else
    0,
#endif
#if defined(GPIOF)
GPIOF,
#else
    0,
#endif
#if defined(GPIOG)
GPIOG,
#else
    0,
#endif
#if defined(GPIOH)
GPIOH,
#else
    0,
#endif
#if defined(GPIOI)
GPIOI,
#else
    0,
#endif
#if defined(GPIOJ)
GPIOJ,
#else
    0,
#endif
#if defined(GPIOK)
GPIOK,
#else
    0,
#endif

    };

int zpin_read(zpin_t zpin)
{
    unsigned long port = gpio_port[zpin >> 4];
    unsigned short pin = (1 << (zpin & 0xf));
    return (GPIO_ISTAT(port) & pin);
}

void zpin_write(zpin_t zpin, int status)
{
    unsigned long port = gpio_port[zpin >> 4];
    unsigned short pin = (1 << (zpin & 0xf));
    if(status)
        GPIO_BOP(port) = pin;
    else
        GPIO_BC(port) = pin;
}
