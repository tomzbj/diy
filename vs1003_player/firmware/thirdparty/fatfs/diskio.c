#include "misc.h"
/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "spiflash.h"

#include "diskio.h"		/* FatFs lower layer API */

/* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define DEV_SPIFLASH 0

#define FLASH_SECTOR_SIZE   4096
#define FLASH_SECTOR_COUNT  512        // SST25VF016, 2M / 4k =
#define FLASH_BLOCK_SIZE    1

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
    DSTATUS stat;
//    int result;

    switch(pdrv) {
#if 0
        case DEV_RAM:
        result = RAM_disk_status();
        // translate the reslut code here
        return stat;
        case DEV_MMC:
        result = MMC_disk_status();
        // translate the reslut code here
        return stat;
        case DEV_USB:
        result = USB_disk_status();
        // translate the reslut code here
        return stat;
#endif
        case DEV_SPIFLASH:
//            result = SPIFLASH_disk_status();
            stat = ~STA_NOINIT;
            // translate the reslut code here
            return stat;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
//    DSTATUS stat;
    int result;

//    uprintf(USART1, "### %d\n", pdrv);
    switch(pdrv) {
#if 0
        case DEV_RAM:
        result = RAM_disk_initialize();
        // translate the reslut code here
        return stat;
        case DEV_MMC:
        result = MMC_disk_initialize();
        // translate the reslut code here
        return stat;
        case DEV_USB:
        result = USB_disk_initialize();
        // translate the reslut code here
        return stat;
#endif
        case DEV_SPIFLASH:
            result = RES_OK;  //SPIFLASH_disk_initialize();
            // translate the reslut code here
            return result;

    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv, /* Physical drive nmuber to identify the drive */
BYTE *buff, /* Data buffer to store read data */
DWORD sector, /* Start sector in LBA */
UINT count /* Number of sectors to read */
)
{
    DRESULT res;
//    int result;

    switch(pdrv) {
#if 0
        case DEV_RAM:
        // translate the arguments here
        result = RAM_disk_read(buff, sector, count);
        // translate the reslut code here
        return res;
        case DEV_MMC:
        // translate the arguments here
        result = MMC_disk_read(buff, sector, count);
        // translate the reslut code here
        return res;
        case DEV_USB:
        // translate the arguments here
        result = USB_disk_read(buff, sector, count);
        // translate the reslut code here
        return res;
#endif
        case DEV_SPIFLASH:
            // translate the arguments here
//            result = SPIFLASH_disk_read(buff, sector, count);
            SPIFLASH_Read(sector * FLASH_SECTOR_SIZE,
                count * FLASH_SECTOR_SIZE, buff);
            res = RES_OK;

            // translate the reslut code here
            return res;
    }

    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#include "usart.h"

DRESULT disk_write(BYTE pdrv, /* Physical drive nmuber to identify the drive */
const BYTE *buff, /* Data to be written */
DWORD sector, /* Start sector in LBA */
UINT count /* Number of sectors to write */
)
{
//    DRESULT res;
//    int result;

//uprintf(USART1, "%d %d\n", __LINE__, pdrv);
    switch(pdrv) {
#if 0
        case DEV_RAM:
        // translate the arguments here
        result = RAM_disk_write(buff, sector, count);
        // translate the reslut code here
        return res;
        case DEV_MMC:
        // translate the arguments here
        result = MMC_disk_write(buff, sector, count);
        // translate the reslut code here
        return res;
        case DEV_USB:
        // translate the arguments here
        result = USB_disk_write(buff, sector, count);
        // translate the reslut code here
        return res;
#endif
        case DEV_SPIFLASH:
            // translate the arguments here
            for(int i = 0; i < count; i++) {
                uprintf(USART1, "%d %d %d\n", sector, i, count);
                if(sector + i >= FLASH_SECTOR_COUNT)
                    return RES_PARERR;
//                FSCMD_viewsect(sector + i);

                SPIFLASH_SectorErase((sector + i) * FLASH_SECTOR_SIZE);
                SPIFLASH_Write((sector + i) * FLASH_SECTOR_SIZE,
                FLASH_SECTOR_SIZE, buff);
//                FSCMD_viewsect(sector + i);

                buff += FLASH_SECTOR_SIZE;

            }
            return RES_OK;
    }

    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
BYTE cmd, /* Control code */
void *buff /* Buffer to send/receive control data */
)
{
    DRESULT res;
//    int result;

    switch(pdrv) {
#if 0
        case DEV_RAM:
        // Process of the command for the RAM drive
        return res;
        case DEV_MMC:
        // Process of the command for the MMC/SD card
        return res;
        case DEV_USB:
        // Process of the command the USB drive
        return res;
#endif
        case DEV_SPIFLASH:
            switch(cmd) {
                case CTRL_SYNC:
                    res = RES_OK;
                    break;
                    /*                case CTRL_ERASE_SECTOR:
                     nFrom = *((DWORD*) buff);
                     nTo = *(((DWORD*) buff) + 1);
                     for(int i = nFrom; i <= nTo; i++)
                     SPIFLASH_SectorErase(i * FLASH_SECTOR_SIZE);
                     break;
                     */
                case GET_SECTOR_SIZE:
                    *(WORD*) buff = FLASH_SECTOR_SIZE;
                    res = RES_OK;
                    break;
                case GET_BLOCK_SIZE:
                    *(DWORD*) buff = FLASH_BLOCK_SIZE;
                    res = RES_OK;
                    break;
                case GET_SECTOR_COUNT:
                    *(DWORD*) buff = FLASH_SECTOR_COUNT;
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
