#include "diskio.h"		// FatFs lower layer API
#include "sd_hal.h"

#define DEV_SPIFLASH 0
#define SECTOR_SIZE   512
#define FLASH_BLOCK_SIZE    1

static DSTATUS sd_ready(void)
{
  return (BSP_SD_GetCardState() == MSD_OK) ? 0 : STA_NOINIT;
}

static DWORD sd_sector_count(void)
{
  HAL_SD_CardInfoTypeDef info = {0};
  BSP_SD_GetCardInfo(&info);
  return info.LogBlockNbr;
}

// Get Drive Status
DSTATUS disk_status(BYTE pdrv)
{
  (void)pdrv;
  return sd_ready();
}

// Inidialize a Drive
DSTATUS disk_initialize(BYTE pdrv)
{
  (void)pdrv;
  return sd_ready();
}

// Read Sector(s)
DRESULT disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
  (void)pdrv;
  if(BSP_SD_ReadBlocks((uint32_t*)buff, sector, count, 1000) != MSD_OK) {
    return RES_ERROR;
  }
  while(BSP_SD_GetCardState() != MSD_OK) {
  }
  return RES_OK;
}

// Write Sector(s)
DRESULT disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
  (void)pdrv;
  if(BSP_SD_WriteBlocks((uint32_t*)buff, sector, count, 1000) != MSD_OK) {
    return RES_ERROR;
  }
  while(BSP_SD_GetCardState() != MSD_OK) {
  }
  return RES_OK;
}

// Miscellaneous Functions
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff)
{
  (void)pdrv;
  if(sd_ready() != 0) {
    return RES_NOTRDY;
  }

  switch(cmd) {
    case CTRL_SYNC:
      return RES_OK;
    case GET_SECTOR_SIZE:
      *(WORD*)buff = SECTOR_SIZE;
      return RES_OK;
    case GET_BLOCK_SIZE:
      *(DWORD*)buff = FLASH_BLOCK_SIZE;
      return RES_OK;
    case GET_SECTOR_COUNT:
      *(DWORD*)buff = sd_sector_count();
      return RES_OK;
    default:
      return RES_PARERR;
  }
}

DWORD get_fattime(void)
{
  return 0;
}
