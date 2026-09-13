#ifndef _EEPROM_H
#define _EEPROM_H

//void EEPROM_Config(void);
void EEPROM_Write(const void* data, int size);
void EEPROM_Read(void* data, int size);

#endif
