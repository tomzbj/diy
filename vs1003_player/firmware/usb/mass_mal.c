#include "mass_mal.h"
#include "spiflash.h"
#include "misc.h"

#include <string.h>

uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];    // used in usb_scsi.c
uint32_t Mass_Block_Count[2];   // used in usb_scsi.c

uint16_t MAL_Init(uint8_t lun)
{
    // init of spiflash
    switch(lun) {
        case 0:
            SPIFLASH_Config();
            return MAL_OK;
        default:
            return MAL_FAIL;

    }
    return MAL_FAIL;
}

uint16_t MAL_GetStatus(uint8_t lun)
{
    switch(lun) {
        case 0:
            Mass_Memory_Size[0] = 16777216UL;
            Mass_Block_Size[0] = 4096UL;
            Mass_Block_Count[0] = Mass_Memory_Size[0] / Mass_Block_Size[0];

            return MAL_OK;
        default:
            return MAL_FAIL;
    }
    return MAL_FAIL;
}

uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff,
    uint16_t Transfer_Length)
{
    switch(lun) {
        case 0:
            SPIFLASH_Read(Memory_Offset, Transfer_Length, Readbuff);
            return MAL_OK;
        default:
            return MAL_FAIL;

    }
    return MAL_FAIL;
}

uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff,
    uint16_t Transfer_Length)
{
    switch(lun) {
        case 0: 
            SPIFLASH_SectorErase(Memory_Offset);
            SPIFLASH_Write(Memory_Offset, Transfer_Length, Writebuff); 
            return MAL_OK;
        default:
            return MAL_FAIL;

    }
    return MAL_FAIL;
}
