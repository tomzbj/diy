#ifndef _DAC_HAL_H
#define _DAC_HAL_H

void DAC_Config(void);
void DAC_DDS_Start(uint16_t* buf, int count);
void DAC_DDS_Stop(void);

#endif
