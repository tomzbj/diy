#ifndef _NRF24LE1_RF_H
#define _NRF24LE1_RF_H

typedef enum {
    RF_CRC_Encoding_1byte = 0, RF_CRC_Encoding_2byte = 1
} RF_CRC_EncodingTypeDef;

typedef enum {
    RF_AutoAck_DataPipe_None = 0x0, RF_AutoAck_DataPipe_0 = 0x1,
    RF_AutoAck_DataPipe_1 = 0x2, RF_AutoAck_DataPipe_2 = 0x4,
    RF_AutoAck_DataPipe_3 = 0x8, RF_AutoAck_DataPipe_4 = 0x10,
    RF_AutoAck_DataPipe_5 = 0x20, RF_AutoAck_DataPipe_All = 0x3f
} RF_AutoAck_DataPipeTypeDef;

typedef enum {
    RF_RxAddr_DataPipe_None = 0x0, RF_RxAddr_DataPipe_0 = 0x1,
    RF_RxAddr_DataPipe_1 = 0x2, RF_RxAddr_DataPipe_2 = 0x4,
    RF_RxAddr_DataPipe_3 = 0x8, RF_RxAddr_DataPipe_4 = 0x10,
    RF_RxAddr_DataPipe_5 = 0x20, RF_RxAddr_DataPipe_All = 0x3f
} RF_RxAddr_DataPipeTypeDef;

typedef enum {
    RF_AddrWidth_3bytes = 1, RF_AddrWidth_4bytes = 2, RF_AddrWidth_5bytes = 3
} RF_AddrWidthTypeDef;

typedef enum {
    RF_RetryDelay_250us = 0, RF_RetryDelay_500us = 1, RF_RetryDelay_750us = 2,
    RF_RetryDelay_1000us = 3, RF_RetryDelay_1250us = 4,
    RF_RetryDelay_1500us = 5, RF_RetryDelay_1750us = 6,
    RF_RetryDelay_2000us = 7, RF_RetryDelay_2250us = 8,
    RF_RetryDelay_2500us = 9, RF_RetryDelay_2750us = 10,
    RF_RetryDelay_3000us = 11, RF_RetryDelay_3250us = 12,
    RF_RetryDelay_3500us = 13, RF_RetryDelay_3750us = 14,
    RF_RetryDelay_4000us = 15
} RF_RetryDelayTypeDef;

typedef enum {
    RF_DataRate_1Mbps = 0, RF_DataRate_2Mbps = 1, RF_DataRate_250kbps = 2
} RF_DataRateTypeDef;

typedef enum {
    RF_Power_N18dBm = 0, RF_Power_N12dBm = 1, RF_Power_N6dBm = 2,
    RF_Power_0dBm = 3
} RF_PowerTypeDef;

typedef enum {
    RF_DynamicPayloadWidth_DataPipe_None = 0x0,
    RF_DynamicPayloadWidth_DataPipe_0 = 0x1,
    RF_DynamicPayloadWidth_DataPipe_1 = 0x2,
    RF_DynamicPayloadWidth_DataPipe_2 = 0x4,
    RF_DynamicPayloadWidth_DataPipe_3 = 0x8,
    RF_DynamicPayloadWidth_DataPipe_4 = 0x10,
    RF_DynamicPayloadWidth_DataPipe_5 = 0x20,
    RF_DynamicPayloadWidth_DataPipe_All = 0x3f
} RF_DynamicPayloadWidth_DataPipeTypeDef;

typedef struct {
    // REG_CONFIG
    unsigned char reserved1 :2;
    unsigned char RF_CRC_Encoding :1;
    unsigned char RF_EN_CRC :1;
    unsigned char RF_Mask_MAXRT :1;
    unsigned char RF_Mask_TXDS :1;
    unsigned char RF_Mask_RXDR :1;
    unsigned char reserved2 :1;
    // REG_ENAA
    unsigned char RF_AutoAck_DataPipe :6;
    unsigned char reserved3 :2;
    // REG_EN_RXADDR
    unsigned char RF_RxAddr_DataPipe :6;
    unsigned char reserved4 :2;
    // REG_SETUP_AW
    unsigned char RF_AddrWidth :2;
    unsigned char reserved5 :6;
    // REG_SETUP_RETR
    unsigned char RF_RetryCount :4;
    unsigned char RF_RetryDelay :4;
    // REG_RF_CH
    unsigned char RF_Channel :7;
    unsigned char reserved6 :1;
    // REG_RF_SETUP
    unsigned char reserved7 :1;
    unsigned char RF_Power :2;
    unsigned char RF_DataRate :2;
    unsigned char reserved8 :3;
    // REG_RX_ADDR_P0
    unsigned char* RF_RxAddr_P0;
    // REG_RX_ADDR_P1
    unsigned char* RF_RxAddr_P1;
    // REG_RX_ADDR_P2
    unsigned char RF_RxAddr_P2LSB;
    // REG_RX_ADDR_P3
    unsigned char RF_RxAddr_P3LSB;
    // REG_RX_ADDR_P4
    unsigned char RF_RxAddr_P4LSB;
    // REG_RX_ADDR_P5
    unsigned char RF_RxAddr_P5LSB;
    // REG_TX_ADDR
    unsigned char* RF_TxAddr;
    // REG_RX_PW_P0
    unsigned char RF_RxPayloadWidth_P0;
    // REG_RX_PW_P1
    unsigned char RF_RxPayloadWidth_P1;
    // REG_RX_PW_P2
    unsigned char RF_RxPayloadWidth_P2;
    // REG_RX_PW_P3
    unsigned char RF_RxPayloadWidth_P3;
    // REG_RX_PW_P4
    unsigned char RF_RxPayloadWidth_P4;
    // REG_RX_PW_P5
    unsigned char RF_RxPayloadWidth_P5;
    // REG＿DYNPD
    unsigned char RF_DynamicPayloadWidth_DataPipe;
    // REG_FEATURE
    unsigned char RF_TxPayloadNoAckCmd :1;
    unsigned char RF_PayloadWithAck :1;
    unsigned char RF_DynamicPayloadWidth :1;
    unsigned char reserved9 :5;
} RF_InitTypeDef;

void RF_Init(RF_InitTypeDef* RF_InitStructure);
void RF_Cmd(FuncState state);

void RF_WriteTxPayload(void *buf, unsigned char len);
void RF_SetAsTx(void);
void RF_SetAsRx(void);
void RF_ReadAllRegs(void);
void RF_FlushTx(void);
void RF_FlushRx(void);
void RF_ClearIRQ(void);
void RF_ReadRxPayload(unsigned char length, unsigned char *rxpl_buf);
unsigned char RF_GetStatus(void);
unsigned char RF_GetFifoStatus(void);

int RF_RX(void* buf, unsigned char len);
int RF_TX(void* buf, unsigned char len);

#endif
