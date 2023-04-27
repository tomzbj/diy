#include "misc.h"
#include "platform.h"

#define DELAY_INTEVAL 120

zi2c i2c(PB8, PB9, 0x70, _delay_us);
shtc3 sht(i2c);

void SHT_Read(void)
{
    shtc3_result_t result = sht.read();
    dc.data.RH = result.RH;
    dc.data.T = result.T;
}
