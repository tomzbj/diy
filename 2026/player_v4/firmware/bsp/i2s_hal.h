#ifndef _I2S_HAL_H
#define _I2S_HAL_H

void I2S_Config(void);
//void I2S_PlayWav(const void* addr, int size);
void I2S_SetSampleRate(uint32_t freq);
void I2S_SetVolume(int volume);
void I2S_VolumeUp(void);
void I2S_VolumeDown(void);

#endif
