#include "nrf24le1.h"

void _delay_ms(unsigned short nms)
{
	unsigned short count; 
	for(count = 0; count < nms; count++) {
		_delay_us(1000);
	}
}

void _delay_us(unsigned short nus)
{
	unsigned short count;

	if(nus == 0)
		return;
	else
		nus--;

	for(count = 0; count < nus ; count++) {
		nop();
		nop();
	}
}
