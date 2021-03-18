#ifndef _MISC_H
#define _MISC_H

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define _dbg() xprintf("%d %s\n", __LINE__, __FILE__)
#define _bitval(x) (1 << (x))

#endif
