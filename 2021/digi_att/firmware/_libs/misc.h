#ifndef _MISC_H
#define _MISC_H

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _dbg() xprintf("%d %s\n", __LINE__, __FILE__)

#endif
