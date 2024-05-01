#ifndef _RF_H
#define _RF_H

#include "nrf24l01.h"

void RF_Config(void);
void RF_Poll(void);
//void RF_ViewRegs(void);
void RF_SetData(uint8_t* data, int size);

extern class NRF24L01 nrf;

#endif
