#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "stm32f10x.h"

#define BULK_MAX_PACKET_SIZE  0x00000040

void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config(FunctionalState NewState);
void Joystick_Send(unsigned char Keys);
unsigned char JoyState(void);

#endif  /*__HW_CONFIG_H*/
