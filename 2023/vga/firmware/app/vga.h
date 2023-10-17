#ifndef _VGA_H
#define _VGA_H

void VGA_Config(void);
void VGA_Clear(void);
void VGA_DrawPixel(unsigned short x, unsigned short y, unsigned long c);

void VGA_Handler(void);

#endif
