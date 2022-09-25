#include "platform.h"
#include "misc.h"

void DISP_Config(void)
{
    LCD_Config();
    LCD_Write("\xff\xff\xff\xff");
//    LCD_WriteInt(1234);

    /*
    while(1) {
        static int count = 0;
        count++;
        count %= 10000;
        LCD_WriteInt(count);
        _delay_ms(500);
//        { int i; for(i = 0; i < 30000; i++); }
    }
    */
}
