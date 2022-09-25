#ifndef _MISC_H
#define _MISC_H

#include "stm8l15x.h"
#include <stdio.h>
//#include "xprintf.h"

#define _delay_ms _delay_ms_systick

#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
void _delay_nms_mm(void);
void _delay_ms_systick(volatile unsigned long nms);


#endif
