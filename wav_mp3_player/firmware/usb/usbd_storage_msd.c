#include "usbd_msc_mem.h"

#include <string.h>
#include "platform.h"

#define STORAGE_LUN_NBR                  1
const int8_t STORAGE_Inquirydata[] = {    // 36
    0x00, 0x80, 0x02, 0x02, (USBD_STD_INQUIRY_LENGTH - 5), 0x00, 0x00, 0x00,
        'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', 'm', 'i', 'c', 'r', 'o', 'S',
        'D', ' ', 'F', 'l', 'a', 's', 'h', ' ', ' ', ' ', '1', '.', '0', '0', };
int8_t STORAGE_Init(uint8_t lun);
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t* block_num,
    uint32_t* block_size);
int8_t STORAGE_IsReady(uint8_t lun);
int8_t STORAGE_IsWriteProtected(uint8_t lun);
int8_t STORAGE_Read(uint8_t lun, uint8_t* buf, uint32_t blk_addr,
    uint16_t blk_len);
int8_t STORAGE_Write(uint8_t lun, uint8_t* buf, uint32_t blk_addr,
    uint16_t blk_len);
int8_t STORAGE_GetMaxLun(void);
USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
    {STORAGE_Init, STORAGE_GetCapacity, STORAGE_IsReady,
        STORAGE_IsWriteProtected, STORAGE_Read, STORAGE_Write,
        STORAGE_GetMaxLun, (int8_t*)STORAGE_Inquirydata, };
USBD_STORAGE_cb_TypeDef* USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops;

//#ifndef USE_STM3210C_EVAL
//extern SD_CardInfo SDCardInfo;
//#endif
__IO uint32_t count = 0;
int8_t STORAGE_Init(uint8_t lun)
{
//    _dbg();
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    SD_Init();
    return (0);
}

int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t* block_num,
    uint32_t* block_size)
{
    SD_CardInfo SDCardInfo;

    SD_GetCardInfo(&SDCardInfo);

    *block_size = 512;
    *block_num = SDCardInfo.CardCapacity / 512;

    return (0);

    return (0);
}
int8_t STORAGE_IsReady(uint8_t lun)
{
//    _dbg();
    static int8_t last_status = 0;

    if(last_status < 0) {
        SD_Init();
        last_status = 0;
    }

    if(SD_GetStatus() != 0) {
        last_status = -1;
        return (-1);
    }
    return (0);
}
int8_t STORAGE_IsWriteProtected(uint8_t lun)
{
//    _dbg();
    return 0;
}
int8_t STORAGE_Read(uint8_t lun, uint8_t* buf, uint32_t blk_addr,
    uint16_t blk_len)
{
//    _dbg();
//    SPIFLASH_FastRead(blk_addr * 4096, blk_len * 4096, buf);
//    memcpy(buf, udisk_buf + blk_addr * 512, blk_len * 512);
//    for(int i = 0; i < blk_len; i++) { SD_ReadBlock(blk_addr * 512, buf, 512); blk_addr++; }
    SD_ReadMultiBlocks(buf, blk_addr * 512, 512, blk_len);
    SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);

    return 0;
}
int8_t STORAGE_Write(uint8_t lun, uint8_t* buf, uint32_t blk_addr,
    uint16_t blk_len)
{
//    _dbg();
//    SPIFLASH_SectorErase(blk_addr * 4096);
//    SPIFLASH_Write(blk_addr * 4096, blk_len * 4096, buf);
    SD_WriteMultiBlocks(buf, blk_addr * 512, 512, blk_len);
    SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);

//    memcpy(udisk_buf + blk_addr * 512, buf, blk_len * 512);
    return (0);
}
int8_t STORAGE_GetMaxLun(void)
{
    return (STORAGE_LUN_NBR - 1);
}
