/* Author: tomzbj  Updated: 2024-04-28  Rev: 0.02 */

#include "zpin.h"

ZPIN_PORT_TYPE ZPin::gpio_port[] = {
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

