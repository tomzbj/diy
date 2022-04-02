#ifndef _SPIFLASH_H
#define _SPIFLASH_H

typedef struct {
    unsigned char (*writebyte_f)(unsigned char);
    void (*cs_f)(int);
    void (*fastread_f)(void* buf, int nbytes);
} spiflash_cfg_t;

unsigned long SPIFLASH_ReadJedecID(void);
void SPIFLASH_Init(spiflash_cfg_t* cfg);
unsigned char SPIFLASH_ReadStatus(void);
int SPIFLASH_Read(unsigned long addr, int nbytes, void* buf);
int SPIFLASH_FastRead(unsigned long addr, int num, void* buf);
int SPIFLASH_Write(unsigned long addr, int nbytes, const void* buf);
int SPIFLASH_Erase(unsigned long addr, int nbytes);
void SPIFLASH_SectorErase(unsigned long addr);
void SPIFLASH_ChipErase(void);

void SPIFLASH_EnterLowPowerMode(void);
void SPIFLASH_LeaveLowPowerMode(void);

#endif
