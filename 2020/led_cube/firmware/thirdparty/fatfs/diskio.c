#include "misc.h"
// Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016
#include "spiflash.h"
#include "diskio.h"		// FatFs lower layer API

#define DEV_SPIFLASH 0 
#define FLASH_SECTOR_SIZE   4096
#define FLASH_SECTOR_COUNT  2048        // USE 512k, 512k / 4096 = 128
#define FLASH_BLOCK_SIZE    1

// Get Drive Status
DSTATUS disk_status(BYTE pdrv) // Physical drive nmuber to identify the drive
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
DSTATUS disk_initialize(BYTE pdrv) // Physical drive nmuber to identify the drive 
{
    int result;
    switch(pdrv) {
        case DEV_SPIFLASH:
            result = RES_OK;  //SPIFLASH_disk_initialize();
            return result;
    }
    return STA_NOINIT;
}

// Read Sector(s)
DRESULT disk_read(BYTE pdrv, // Physical drive nmuber to identify the drive 
    BYTE *buff, // Data buffer to store read data
    DWORD sector, // Start sector in LBA
    UINT count // Number of sectors to read
    )
{
    DRESULT res;

    switch(pdrv) {
        case DEV_SPIFLASH:
            SPIFLASH_Read(sector * FLASH_SECTOR_SIZE,
                count * FLASH_SECTOR_SIZE, buff);
            res = RES_OK;
            return res;
    }
    return RES_PARERR;
}

// Write Sector(s)
DRESULT disk_write(BYTE pdrv, // Physical drive nmuber to identify the drive
    const BYTE *buff, // Data to be written
    DWORD sector, // Start sector in LBA
    UINT count // Number of sectors to write
    )
{
    switch(pdrv) {
        case DEV_SPIFLASH:
            // translate the arguments here
            for(int i = 0; i < count; i++) {
//                uprintf(USART1, "%d %d %d\n", sector, i, count);
                if(sector + i >= FLASH_SECTOR_COUNT)
                    return RES_PARERR;
                SPIFLASH_SectorErase((sector + i) * FLASH_SECTOR_SIZE);
                SPIFLASH_Write((sector + i) * FLASH_SECTOR_SIZE,
                FLASH_SECTOR_SIZE, buff);
                buff += FLASH_SECTOR_SIZE;
            }
            return RES_OK;
    }
    return RES_PARERR;
}

// Miscellaneous Functions
DRESULT disk_ioctl(BYTE pdrv, // Physical drive nmuber (0..)
    BYTE cmd, // Control code
    void *buff // Buffer to send/receive control data
    )
{
    DRESULT res;
    switch(pdrv) {
        case DEV_SPIFLASH:
            switch(cmd) {
                case CTRL_SYNC:
                    res = RES_OK;
                    break;
                    /*                case CTRL_ERASE_SECTOR:
                    nFrom = *((DWORD*)buff);
                    nTo = *(((DWORD*)buff) + 1);
                    for(int i = nFrom; i <= nTo; i++)
                        SPIFLASH_SectorErase(i * FLASH_SECTOR_SIZE);
                    break;
                    */
                case GET_SECTOR_SIZE:
                    *(WORD*)buff = FLASH_SECTOR_SIZE;
                    res = RES_OK;
                    break;
                case GET_BLOCK_SIZE:
                    *(DWORD*)buff = FLASH_BLOCK_SIZE;
                    res = RES_OK;
                    break;
                case GET_SECTOR_COUNT:
                    *(DWORD*)buff = FLASH_SECTOR_COUNT;
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
