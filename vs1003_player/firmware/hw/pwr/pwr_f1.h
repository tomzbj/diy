#ifndef _PWR_F1_H
#define _PWR_F1_H

typedef enum {
    // PB11, PF7, PB5, PA12
    PWR_LCD_BackLight, PWR_CUR, PWR_VDD_VCCA, PWR_BATT_TEST
} PWR_CtrlTypeDef;

//void PWR_Config(void);
//void PWR_Ctrl(PWR_CtrlTypeDef dev, FunctionalState state);
void PWR_Sleep(void);

#endif /* PWR_H_ */
