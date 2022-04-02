#include <string.h>
#include "misc.h"
#include "dataconfig.h"

#define EEPROM_BASE_ADDR 0x4000     // for stm8s003 & stm8s103

static DataConfig_t g;

static void EEPROM_Read(unsigned long addr, void* buf, int size)
{
    memcpy(buf, (void*)(addr + EEPROM_BASE_ADDR), size);
}

static void EEPROM_Write(unsigned long addr, void* buf, int size)
{
    disableInterrupts();
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    while(FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET) ;
	memcpy((void*)(addr + EEPROM_BASE_ADDR), buf, size);
    FLASH_Lock(FLASH_MEMTYPE_DATA);
    enableInterrupts();
}

void DC_Config(void)
{
    EEPROM_Read(0, &(g.cfg), sizeof(g.cfg));
}

void DC_Update(void)
{
    unsigned char buf[sizeof(g.cfg)];
    EEPROM_Read(0, buf, sizeof(g.cfg));
    if(memcmp(&(g.cfg), buf, sizeof(g.cfg)) != 0) {
        EEPROM_Write(0, &(g.cfg), sizeof(g.cfg));
    }
}

DataConfig_t* DC_Get(void)
{
    return &g;
}
