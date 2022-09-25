#ifndef _LCD_H
#define _LCD_H

void LCD_Config(void);
void LCD_Write(unsigned char data[]);
void LCD_Clear(void);
void LCD_WriteFloat(float n);
void LCD_WriteInt(unsigned int n);
void LCD_WriteTime(unsigned long sec_of_day);
void LCD_SetBlinkingSegments(unsigned char seg);

#endif
