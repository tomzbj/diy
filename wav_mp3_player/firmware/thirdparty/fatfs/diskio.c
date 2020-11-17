#include "misc.h"
#include "platform.h"
// Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016
#include "diskio.h"		// FatFs lower layer API

#define DEV_SPIFLASH 0 
#define SD_SECTOR_SIZE   512
#define FLASH_SECTOR_COUNT  128        // USE 512k, 512k / 4096 = 128
#define FLASH_BLOCK_SIZE    1

// Get Drive Status
DSTATUS disk_status(BYTE pdrv)    // Physical drive nmuber to identify the drive
{
    return RES_OK;
    DSTATUS stat;

    switch(pdrv) {
        case DEV_SPIFLASH:
            stat = ~STA_NOINIT;
            return stat;
    }
    return STA_NOINIT;
}

// Inidialize a Drive
DSTATUS disk_initialize(BYTE pdrv)    // Physical drive nmuber to identify the drive
{
    int result;
    switch(pdrv) {
        case DEV_SPIFLASH:
            result = RES_OK;    //SPIFLASH_disk_initialize();
            return result;
    }
    return STA_NOINIT;
}

// Read Sector(s)
DRESULT disk_read(BYTE pdrv,    // Physical drive nmuber to identify the drive
    BYTE* buff,    // Data buffer to store read data
    DWORD sector,    // Start sector in LBA
    UINT count    // Number of sectors to read
    )
{
    DRESULT res;

    switch(pdrv) {
        case DEV_SPIFLASH: {
            if(count > 1)
                SD_ReadMultiBlocks(buff, sector * SD_SECTOR_SIZE,
                SD_SECTOR_SIZE, count);
            else
                SD_ReadBlock(buff, sector * SD_SECTOR_SIZE, SD_SECTOR_SIZE);
            SD_WaitReadOperation();
            while(SD_GetStatus() != SD_TRANSFER_OK);
        }
            res = RES_OK;
            return res;
    }
    return RES_PARERR;
}

// Write Sector(s)
DRESULT disk_write(BYTE pdrv,    // Physical drive nmuber to identify the drive
    const BYTE* buff,    // Data to be written
    DWORD sector,    // Start sector in LBA
    UINT count    // Number of sectors to write
    )
{
    switch(pdrv) {
        case DEV_SPIFLASH: {
            if(count > 1)
                SD_WriteMultiBlocks((BYTE*)buff, sector * SD_SECTOR_SIZE,
                    SD_SECTOR_SIZE, count);
            else
                SD_WriteBlock((BYTE*)buff, sector * SD_SECTOR_SIZE,
                    SD_SECTOR_SIZE);
            SD_WaitWriteOperation();
            while(SD_GetStatus() != SD_TRANSFER_OK);
        }
            return RES_OK;
    }
    return RES_PARERR;
}

// Miscellaneous Functions
DRESULT disk_ioctl(BYTE pdrv,    // Physical drive nmuber (0..)
    BYTE cmd,    // Control code
    void* buff    // Buffer to send/receive control data
    )
{
    DRESULT res;
    switch(pdrv) {
        case DEV_SPIFLASH:
            switch(cmd) {
                case CTRL_SYNC:
                    res = RES_OK;
                    break;
                case GET_SECTOR_SIZE:
                    *(WORD*)buff = SD_SECTOR_SIZE;
                    res = RES_OK;
                    break;
                case GET_BLOCK_SIZE:
                    *(DWORD*)buff = SD_SECTOR_SIZE;
                    res = RES_OK;
                    break;
                case GET_SECTOR_COUNT: {
                    SD_CardInfo SDCardInfo;
                    SD_GetCardInfo(&SDCardInfo);
                    *(DWORD*)buff = SDCardInfo.CardCapacity / 512;
                }
                    res = RES_OK;
                    break;
                default:
                    res = RES_PARERR;
                    break;
            }
            return res;
    }
    return RES_PARERR;
}

DWORD get_fattime(void)
{
    return 0;
}
