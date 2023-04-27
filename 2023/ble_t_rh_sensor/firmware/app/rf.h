#ifndef _RF_H
#define _RF_H

#include "nrf24l01.h"

void RF_Config(void);
void RF_Poll(void);

extern class nrf24l01 nrf;
extern class nrf24l01 nrf2;

#endif
