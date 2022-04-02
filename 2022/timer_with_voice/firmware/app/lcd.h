#ifndef _LCD_H
#define _LCD_H

void LCD_Config(void);
void LCD_WriteRaw(unsigned char buf[]);
void LCD_WriteInt(int n);
void LCD_WriteDots(unsigned char dots);
void LCD_Update(void);

#endif
