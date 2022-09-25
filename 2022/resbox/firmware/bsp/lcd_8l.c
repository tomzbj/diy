#include "misc.h"
#include "platform.h"

//volatile unsigned char g_blinking_seg = 0x00;
//void LCD_SetBlinkingSegments(unsigned char seg) { g_blinking_seg = seg; }
const unsigned char g_mask[] = { //
    0xb7, 0x22, 0x9b, 0xab, 0x2e, 0xad, 0xbd, 0x23, 0xbf, 0xaf    //
    };

void LCD_Write(unsigned char data[])
{
    LCD_WriteRAM(LCD_RAMRegister_0,
        ((data[3] & 0x3) << 6) | ((data[2] & 0x3) << 4)
            | ((data[1] & 0x3) << 2) | (data[0] & 0x3));
    LCD_WriteRAM(LCD_RAMRegister_3,
        ((data[1] & 0xc) << 4) | ((data[0] & 0xc) << 2));
    LCD_WriteRAM(LCD_RAMRegister_4, (data[3] & 0xc) | ((data[2] & 0xc) >> 2));
    LCD_WriteRAM(LCD_RAMRegister_7,
        ((data[3] & 0x30) << 2) | (data[2] & 0x30) | ((data[1] & 0x30) >> 2)
            | ((data[0] & 0x30) >> 4));
    LCD_WriteRAM(LCD_RAMRegister_10,
        (data[1] & 0xc0) | ((data[0] & 0xc0) >> 2));
    LCD_WriteRAM(LCD_RAMRegister_11,
        ((data[3] & 0xc0) >> 4) | ((data[2] & 0xc0) >> 6));
}

void LCD_Clear(void)
{
    LCD_Write("\x00\x00\x00\x00");
}

void LCD_Config(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2);
    CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);

    LCD_Init(LCD_Prescaler_1, LCD_Divider_16, LCD_Duty_1_4, LCD_Bias_1_3,
        LCD_VoltageSource_Internal);    // Init

    LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xff);    // ENABLE SEG0~7
//    LCD_ContrastConfig(LCD_Contrast_3V3);    // Only available for internal voltage
    LCD_ContrastConfig(LCD_Contrast_Level_3);
    LCD_DeadTimeConfig(LCD_DeadTime_3);
    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_3);
    LCD_Cmd(ENABLE);
}

/*
 void LCD_WriteFloat(float n)
 {
 int i, dot = 1;
 float n2 = n;
 unsigned char buf[4] = {0};

 while(n2 >= 10.0) {
 n2 /= 10.0;
 dot++;
 }

 for(i = 0; i <= 3; i++) {
 buf[i] = g_mask[(int)n2];
 n2 = (n2 - (int)n2) * 10.0;
 }
 if(dot <= 3)
 buf[dot] |= 0x40;
 LCD_Write(buf);
 }
 */

void LCD_WriteInt(unsigned int n)
{
    int i;
    unsigned char buf[4] = {0};

    for(i = 3; n > 0 && i >= 0; i--) {
        buf[i] = g_mask[n % 10];
        n /= 10;
    }
    if(i == 3)
        buf[1] = buf[2] = buf[3] = 0xb7;
    LCD_Write(buf);
}
