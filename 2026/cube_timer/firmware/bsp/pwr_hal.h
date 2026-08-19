#ifndef _PWR_HAL_H
#define _PWR_HAL_H

void PWR_Config(void);
void PWR_ClearFlags(void);

int PWR_FlagStandby(void);
int PWR_FlagWkup(void);

// PA0 / WKUP pin level (also MMA7660 INT)
int PWR_ReadWkupPin(void);

// Enable PA0 WKUP (rising), enter STANDBY; does not return on success
void PWR_EnterStandby(void);

void PWR_EN_LDO(void);

#endif
