#ifndef __STM324xG_EVAL_SDIO_SD_H
#define __STM324xG_EVAL_SDIO_SD_H

#include <stdint.h>
#include "misc.h"

#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40012C80)


#define SDIO_INIT_CLK_DIV                ((uint8_t)0x76)
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x0)

#define SD_DETECT_PIN                    GPIO_Pin_13                 /* PH.13 */
#define SD_DETECT_GPIO_PORT              GPIOH                       /* GPIOH */
#define SD_DETECT_GPIO_CLK               RCC_AHB1Periph_GPIOH

#define SD_SDIO_DMA                   DMA2
#define SD_SDIO_DMA_CLK               RCC_AHB1Periph_DMA2

#define SD_SDIO_DMA_STREAM3           3
//#define SD_SDIO_DMA_STREAM6           6

#ifdef SD_SDIO_DMA_STREAM3
 #define SD_SDIO_DMA_STREAM            DMA2_Stream3
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF3
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF3
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF3
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF3
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF3
 #define SD_SDIO_DMA_IRQn              DMA2_Stream3_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream3_IRQHandler
#elif defined SD_SDIO_DMA_STREAM6
 #define SD_SDIO_DMA_STREAM            DMA2_Stream6
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF6
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF6
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF6
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF6
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF6
 #define SD_SDIO_DMA_IRQn              DMA2_Stream6_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream6_IRQHandler
#endif /* SD_SDIO_DMA_STREAM3 */

typedef enum {
    SD_CMD_CRC_FAIL = (1),
    SD_DATA_CRC_FAIL = (2),
    SD_CMD_RSP_TIMEOUT = (3),
    SD_DATA_TIMEOUT = (4),
    SD_TX_UNDERRUN = (5),
    SD_RX_OVERRUN = (6),
    SD_START_BIT_ERR = (7),
    SD_CMD_OUT_OF_RANGE = (8),
    SD_ADDR_MISALIGNED = (9),
    SD_BLOCK_LEN_ERR = (10),
    SD_ERASE_SEQ_ERR = (11),
    SD_BAD_ERASE_PARAM = (12),
    SD_WRITE_PROT_VIOLATION = (13),
    SD_LOCK_UNLOCK_FAILED = (14),
    SD_COM_CRC_FAILED = (15),
    SD_ILLEGAL_CMD = (16),
    SD_CARD_ECC_FAILED = (17),
    SD_CC_ERROR = (18),
    SD_GENERAL_UNKNOWN_ERROR = (19),
    SD_STREAM_READ_UNDERRUN = (20),
    SD_STREAM_WRITE_OVERRUN = (21),
    SD_CID_CSD_OVERWRITE = (22),
    SD_WP_ERASE_SKIP = (23),
    SD_CARD_ECC_DISABLED = (24),
    SD_ERASE_RESET = (25),
    SD_AKE_SEQ_ERROR = (26),
    SD_INVALID_VOLTRANGE = (27),
    SD_ADDR_OUT_OF_RANGE = (28),
    SD_SWITCH_ERROR = (29),
    SD_SDIO_DISABLED = (30),
    SD_SDIO_FUNCTION_BUSY = (31),
    SD_SDIO_FUNCTION_FAILED = (32),
    SD_SDIO_UNKNOWN_FUNCTION = (33),
    SD_INTERNAL_ERROR,
    SD_NOT_CONFIGURED,
    SD_REQUEST_PENDING,
    SD_REQUEST_NOT_APPLICABLE,
    SD_INVALID_PARAMETER,
    SD_UNSUPPORTED_FEATURE,
    SD_UNSUPPORTED_HW,
    SD_ERROR,
    SD_OK = 0
} SD_Error;
typedef enum {
    SD_TRANSFER_OK = 0,
    SD_TRANSFER_BUSY = 1,
    SD_TRANSFER_ERROR
} SDTransferState;
typedef enum {
    SD_CARD_READY = ((uint32_t) 0x00000001),
    SD_CARD_IDENTIFICATION = ((uint32_t) 0x00000002),
    SD_CARD_STANDBY = ((uint32_t) 0x00000003),
    SD_CARD_TRANSFER = ((uint32_t) 0x00000004),
    SD_CARD_SENDING = ((uint32_t) 0x00000005),
    SD_CARD_RECEIVING = ((uint32_t) 0x00000006),
    SD_CARD_PROGRAMMING = ((uint32_t) 0x00000007),
    SD_CARD_DISCONNECTED = ((uint32_t) 0x00000008),
    SD_CARD_ERROR = ((uint32_t) 0x000000FF)
} SDCardState;
typedef struct {
    __IO uint8_t CSDStruct;
    __IO uint8_t SysSpecVersion;
    __IO uint8_t Reserved1;
    __IO uint8_t TAAC;
    __IO uint8_t NSAC;
    __IO uint8_t MaxBusClkFrec;
    __IO uint16_t CardComdClasses;
    __IO uint8_t RdBlockLen;
    __IO uint8_t PartBlockRead;
    __IO uint8_t WrBlockMisalign;
    __IO uint8_t RdBlockMisalign;
    __IO uint8_t DSRImpl;
    __IO uint8_t Reserved2;
    __IO uint32_t DeviceSize;
    __IO uint8_t MaxRdCurrentVDDMin;
    __IO uint8_t MaxRdCurrentVDDMax;
    __IO uint8_t MaxWrCurrentVDDMin;
    __IO uint8_t MaxWrCurrentVDDMax;
    __IO uint8_t DeviceSizeMul;
    __IO uint8_t EraseGrSize;
    __IO uint8_t EraseGrMul;
    __IO uint8_t WrProtectGrSize;
    __IO uint8_t WrProtectGrEnable;
    __IO uint8_t ManDeflECC;
    __IO uint8_t WrSpeedFact;
    __IO uint8_t MaxWrBlockLen;
    __IO uint8_t WriteBlockPaPartial;
    __IO uint8_t Reserved3;
    __IO uint8_t ContentProtectAppli;
    __IO uint8_t FileFormatGrouop;
    __IO uint8_t CopyFlag;
    __IO uint8_t PermWrProtect;
    __IO uint8_t TempWrProtect;
    __IO uint8_t FileFormat;
    __IO uint8_t ECC;
    __IO uint8_t CSD_CRC;
    __IO uint8_t Reserved4;
} SD_CSD;
typedef struct {
    __IO uint8_t ManufacturerID;
    __IO uint16_t OEM_AppliID;
    __IO uint32_t ProdName1;
    __IO uint8_t ProdName2;
    __IO uint8_t ProdRev;
    __IO uint32_t ProdSN;
    __IO uint8_t Reserved1;
    __IO uint16_t ManufactDate;
    __IO uint8_t CID_CRC;
    __IO uint8_t Reserved2;
} SD_CID;
typedef struct {
    __IO uint8_t DAT_BUS_WIDTH;
    __IO uint8_t SECURED_MODE;
    __IO uint16_t SD_CARD_TYPE;
    __IO uint32_t SIZE_OF_PROTECTED_AREA;
    __IO uint8_t SPEED_CLASS;
    __IO uint8_t PERFORMANCE_MOVE;
    __IO uint8_t AU_SIZE;
    __IO uint16_t ERASE_SIZE;
    __IO uint8_t ERASE_TIMEOUT;
    __IO uint8_t ERASE_OFFSET;
} SD_CardStatus;
typedef struct {
    SD_CSD SD_csd;
    SD_CID SD_cid;
    uint64_t CardCapacity;
    uint32_t CardBlockSize;
    uint16_t RCA;
    uint8_t CardType;
} SD_CardInfo;
#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3)
#define SD_CMD_SET_DSR                             ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SD_CMD_SEND_CSD                            ((uint8_t)9)
#define SD_CMD_SEND_CID                            ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11)
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20)
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23)
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SD_CMD_PROG_CID                            ((uint8_t)26)
#define SD_CMD_PROG_CSD                            ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32)
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33)
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35)
#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36)
#define SD_CMD_ERASE                               ((uint8_t)38)
#define SD_CMD_FAST_IO                             ((uint8_t)39)
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40)
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)
#define SD_CMD_APP_CMD                             ((uint8_t)55)
#define SD_CMD_GEN_CMD                             ((uint8_t)56)
#define SD_CMD_NO_CMD                              ((uint8_t)64)
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)
#define SD_CMD_SD_APP_STAUS                        ((uint8_t)13)
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22)
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41)
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42)
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51)
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8_t)52)
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8_t)53)
#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43)
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44)
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45)
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46)
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47)
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48)
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18)
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25)
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38)
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49)
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48)

#define SD_DMA_MODE

#if !defined (SD_DMA_MODE) && !defined (SD_POLLING_MODE)
#define SD_DMA_MODE                                ((uint32_t)0x00000000)
#endif
#define SD_PRESENT                                 ((uint8_t)0x01)
#define SD_NOT_PRESENT                             ((uint8_t)0x00)
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

void SD_DeInit(void);
SD_Error SD_Init(void);
SDTransferState SD_GetStatus(void);
SDCardState SD_GetState(void);
uint8_t SD_Detect(void);
SD_Error SD_PowerON(void);
SD_Error SD_PowerOFF(void);
SD_Error SD_InitializeCards(void);
SD_Error SD_GetCardInfo(SD_CardInfo * cardinfo);
SD_Error SD_GetCardStatus(SD_CardStatus * cardstatus);
SD_Error SD_EnableWideBusOperation(uint32_t WideMode);
SD_Error SD_SelectDeselect(uint64_t addr);
SD_Error SD_ReadBlock(uint8_t * readbuff, uint64_t ReadAddr,
                      uint16_t BlockSize);
SD_Error SD_ReadMultiBlocks(uint8_t * readbuff, uint64_t ReadAddr,
                            uint16_t BlockSize, uint32_t NumberOfBlocks);
SD_Error SD_WriteBlock(uint8_t * writebuff, uint64_t WriteAddr,
                       uint16_t BlockSize);
SD_Error SD_WriteMultiBlocks(uint8_t * writebuff, uint64_t WriteAddr,
                             uint16_t BlockSize, uint32_t NumberOfBlocks);
SDTransferState SD_GetTransferState(void);
SD_Error SD_StopTransfer(void);
SD_Error SD_Erase(uint64_t startaddr, uint64_t endaddr);
SD_Error SD_SendStatus(uint32_t * pcardstatus);
SD_Error SD_SendSDStatus(uint32_t * psdstatus);
SD_Error SD_ProcessIRQSrc(void);
void SD_ProcessDMAIRQ(void);
SD_Error SD_WaitReadOperation(void);
SD_Error SD_WaitWriteOperation(void);
SD_Error SD_HighSpeed(void);

void SD_LowLevel_DeInit(void);

#endif
