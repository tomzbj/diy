#ifndef _DISP_H
#define _DISP_H

void LCD_WriteReg(unsigned short x);
void LCD_WriteData(unsigned short x);
unsigned short LCD_ReadData(void);
void LCD_WriteDataFast(unsigned short* buf, int n);
void LCD_ReadDataFast(unsigned short* buf, int n);

void DISP_Config(void);
void DISP_Clear(void);
void DISP_Poll(void);
void DISP_DrawPixel(unsigned short x, unsigned short y, unsigned long c);
void DISP_Off(void);
void DISP_ToggleUnit(void);

#endif
