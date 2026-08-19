#ifndef _ADC_HAL_H
#define _ADC_HAL_H

void ADC_Config(void);

// PA11 OD: 1 = enable divider (drive low), 0 = cut off (release high)
void ADC_DividerEnable(int enable);

// Read pack voltage in volts (2x 100k divider on PA2). Divider should be enabled.
float ADC_ReadVbat(void);

// Call before STANDBY — PA11 high so pack does not bleed through divider
void ADC_PrepareStandby(void);

#endif
