/* spiflash.h
 * r0.2
 * Zhang Hao, stavrosatic@gmail.com
 * 2018.7.11
 */

#include "spiflash_port.h"

#ifndef _SPIFLASH_H
#define _SPIFLASH_H

typedef struct {
    unsigned char device_id;
    unsigned char manufacturer_id;
    unsigned char memory_type;
    unsigned char capacity;
} SPIFLASH_Info_t;

typedef unsigned char (*spiflash_writebyte_func_t)(unsigned char);
typedef void (*spiflash_cs_func_t)(int);

#define SPIFLASH_MODEL_SST25VF016  10001

#define SPIFLASH_MODEL SPIFLASH_MODEL_SST25VF016

SPIFLASH_Info_t* SPIFLASH_ReadInfo(void);
void SPIFLASH_Init(spiflash_writebyte_func_t spiflash_writebyte_func,
    spiflash_cs_func_t spiflash_cs_func);

unsigned char SPIFLASH_ReadStatus(void);
int SPIFLASH_Read(unsigned long addr, int num, void* buf);
int SPIFLASH_Write(unsigned long addr, int num, const void* buf);
int SPIFLASH_Erase(unsigned long addr, unsigned long n);
void SPIFLASH_SectorErase(unsigned long addr);
void SPIFLASH_ChipErase(void);


void SPIFLASH_EnterLowPowerMode(void);
void SPIFLASH_LeaveLowPowerMode(void);

#endif
