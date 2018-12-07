#include "nrf24le1.h"
#include <stdio.h>

#define SET_CSN  RFCON |= _BV(1)
#define CLR_CSN  RFCON &= ~_BV(1)
#define SET_CE   RFCON |= _BV(0)
#define CLR_CE   RFCON &= ~_BV(0)

#define CMD_READ_REG    0x00    // 读配置寄存器,低5位为寄存器地址
#define CMD_WRITE_REG   0x20    // 写配置寄存器,低5位为寄存器地址
#define CMD_READ_RX_PL  0x61    // 读RX有效数据,1~32字节
#define CMD_WRITE_TX_PL 0xa0    // 写TX有效数据,1~32字节
#define CMD_FLUSH_TX    0xe1    // 清除TX FIFO寄存器.发射模式下用
#define CMD_FLUSH_RX    0xe2    // 清除RX FIFO寄存器.接收模式下用
#define CMD_REUSE_TX_PL 0xe3    // 重新使用上一包数据,CE为高,数据包被不断发送.
#define CMD_READ_RX_PL_WIDTH 0x60 // R_RX_PL_WID
#define CMD_WRITE_ACK_PL 0xa8   // W_ACK_PAYLOAD, +3bit ACK pipe no(000 to 101)
#define CMD_WRITE_TX_PL_NOACK 0xb0 // W_TX_PAYLOAD_NO_AC
#define CMD_NOP         0xff    // 空操作,可以用来读状态寄存器

#define REG_CONFIG      0x00    // 配置寄存器地址
#define REG_EN_AA       0x01    // 使能自动应答功能
#define REG_EN_RXADDR   0x02    // 接收地址允许
#define REG_SETUP_AW    0x03    // 设置地址宽度(所有数据通道)
#define REG_SETUP_RETR  0x04    // 建立自动重发
#define REG_RF_CH       0x05    // RF通道
#define REG_RF_SETUP    0x06    // RF寄存器
#define REG_STATUS      0x07    // 状态寄存器
#define REG_OBSERVE_TX  0x08    // 发送检测寄存器
#define REG_CD          0x09    // 载波检测寄存器
#define REG_RX_ADDR_P0  0x0a    // 数据通道0接收地址
#define REG_RX_ADDR_P1  0x0b    // 数据通道1接收地址
#define REG_RX_ADDR_P2  0x0c    // 数据通道2接收地址
#define REG_RX_ADDR_P3  0x0d    // 数据通道3接收地址
#define REG_RX_ADDR_P4  0x0e    // 数据通道4接收地址
#define REG_RX_ADDR_P5  0x0f    // 数据通道5接收地址
#define REG_TX_ADDR     0x10    // 发送地址寄存器
#define REG_RX_PW_P0    0x11    // 接收数据通道0有效数据宽度(1~32字节)
#define REG_RX_PW_P1    0x12    // 接收数据通道1有效数据宽度(1~32字节)
#define REG_RX_PW_P2    0x13    // 接收数据通道2有效数据宽度(1~32字节)
#define REG_RX_PW_P3    0x14    // 接收数据通道3有效数据宽度(1~32字节)
#define REG_RX_PW_P4    0x15    // 接收数据通道4有效数据宽度(1~32字节)
#define REG_RX_PW_P5    0x16    // 接收数据通道5有效数据宽度(1~32字节)
#define REG_FIFO_STATUS 0x17    // FIFO状态寄存器
#define REG_DYNPD       0x1c    // enable dynamic payload length in data pipes
#define REG_FEATURE     0x1d    // global feature reg

#define AD_LEN          5
#define TX_PAYLOAD_WIDTH 30

#define ReadByte() WriteByte(0xff)

unsigned char WriteByte(unsigned char x)
{
    SPIRDAT = x;
    while(!(SPIRSTAT & _BV(2)))
        ;
    IRCON &= ~_BV(0);
    return SPIRDAT;
}

void WriteReg(unsigned char reg, unsigned char val)
{
    CLR_CSN;
    WriteByte(reg | CMD_WRITE_REG);
    WriteByte(val);
    SET_CSN;
}

unsigned char ReadReg(unsigned char reg)
{
    unsigned char val;
    CLR_CSN;
    WriteByte(reg);
    val = ReadByte();
    SET_CSN;

    return val;
}

unsigned char WriteData(unsigned char reg, unsigned char *buf,
        unsigned char len)
{
    unsigned char status, i;
    CLR_CSN;
    status = WriteByte(CMD_WRITE_REG | reg);
    for(i = 0; i < len; i++)
        WriteByte(buf[i]);
    SET_CSN;
    return status;
}

unsigned char ReadData(unsigned char reg, unsigned char *buf,
        unsigned char len)
{
    unsigned char status, i;
    CLR_CSN;
    status = WriteByte(reg);
    for(i = 0; i < len; i++)
        buf[i] = ReadByte();
    SET_CSN;
    return status;
}

// 读24L01的RX:5Payload
// 参数: 读出长度length, 缓冲区指针rxpl_buf 返回值:    状态
void RF_ReadRxPayload(unsigned char length, unsigned char *rxpl_buf)
{
    int i;
    CLR_CSN;
    WriteByte(CMD_READ_RX_PL);
    for(i = 0; i < length; i++) {
        rxpl_buf[i] = ReadByte();
    }
    SET_CSN;
}

// 写24L01的TX Payload
// 参数:       读出长度length, 缓冲区指针txpl_buf 返回值:    状态
void RF_WriteTxPayload(void* txpl_buf, unsigned char len)
{
    WriteData(CMD_WRITE_TX_PL_NOACK, txpl_buf, len);
}

//刷新24L01的TX FIFO
void RF_FlushTx(void)
{
    CLR_CSN;
    WriteByte(CMD_FLUSH_TX);
    SET_CSN;
}

//刷新24L01的RX FIFO
void RF_FlushRx(void)
{
//    unsigned char status;
    CLR_CSN;
    WriteByte(CMD_FLUSH_RX);
//    status = WriteByte(CMD_FLUSH_RX);
    SET_CSN;
//    return status;
}

unsigned char ReuseTxPayload(void)
{
    unsigned char status;
    CLR_CSN;
    status = WriteByte(CMD_REUSE_TX_PL);
    SET_CSN;
    return status;
}

unsigned char ReadRxPayloadWidth(unsigned char length,
        unsigned char *txpl_width)
{
    unsigned char status;
    length = length;
    CLR_CSN;
    status = WriteByte(CMD_READ_RX_PL_WIDTH);
    txpl_width[0] = ReadByte();
    SET_CSN;
    return status;
}

unsigned char WriteAckTxPayload(unsigned char pipe, unsigned char length,
        unsigned char * txpl_buf)
{
    unsigned char status, i;
    CLR_CSN;
    status = WriteByte(CMD_WRITE_ACK_PL + pipe);
    for(i = 0; i < length; i++) {
        WriteByte(txpl_buf[i]);
    }
    SET_CSN;
    return status;
}

unsigned char WriteNoAckTxPayload(unsigned char length,
        unsigned char * txpl_buf)
{
    unsigned char status, i;
    CLR_CSN;
    status = WriteByte(CMD_WRITE_TX_PL_NOACK);
    for(i = 0; i < length; i++) {
        WriteByte(txpl_buf[i]);
    }
    SET_CSN;
    return status;
}

unsigned char RF_GetStatus(void)
{
    return ReadReg(REG_STATUS);
}

unsigned char RF_GetFifoStatus(void)
{
    return ReadReg(REG_FIFO_STATUS);
}

void RF_ClearIRQ(void)
{
    WriteReg(REG_STATUS, 0xff);
}

//todo: 设置channel, data rate等常量的函数
void RF_SetAsTx(void)
{
    unsigned char reg = ReadReg(REG_CONFIG);
    reg &= ~_BV(0);
    WriteReg(REG_CONFIG, reg);
}

void RF_SetAsRx(void)
{
    unsigned char reg = ReadReg(REG_CONFIG);
    reg |= _BV(0);
    WriteReg(REG_CONFIG, reg);
//    WriteReg(REG_CONFIG, 0x0f);
}

void RF_Cmd(FuncState state)
{
    unsigned char reg = ReadReg(REG_CONFIG);
    if(state == ENABLE)
        reg |= _BV(1);
    else
        reg &= ~_BV(1);
    WriteReg(REG_CONFIG, reg);
}

/*
void RF_ReadAllRegs(void)
{
    printf("\n00-07: ");
    printf("%02x ", ReadReg(REG_CONFIG));
    printf("%02x ", ReadReg(REG_EN_AA));
    printf("%02x ", ReadReg(REG_EN_RXADDR));
    printf("%02x ", ReadReg(REG_SETUP_AW));
    printf("%02x ", ReadReg(REG_SETUP_RETR));
    printf("%02x ", ReadReg(REG_RF_CH));
    printf("%02x ", ReadReg(REG_RF_SETUP));
    printf("%02x ", ReadReg(REG_STATUS));

    printf("\n08-0f: ");
    printf("%02x ", ReadReg(REG_OBSERVE_TX));
    printf("%02x ", ReadReg(REG_CD));
    printf("%02x ", ReadReg(REG_RX_ADDR_P0));
    printf("%02x ", ReadReg(REG_RX_ADDR_P1));
    printf("%02x ", ReadReg(REG_RX_ADDR_P2));
    printf("%02x ", ReadReg(REG_RX_ADDR_P3));
    printf("%02x ", ReadReg(REG_RX_ADDR_P4));
    printf("%02x ", ReadReg(REG_RX_ADDR_P5));

    printf("\n10-17: ");
    printf("%02x ", ReadReg(REG_TX_ADDR));
    printf("%02x ", ReadReg(REG_RX_PW_P0));
    printf("%02x ", ReadReg(REG_RX_PW_P1));
    printf("%02x ", ReadReg(REG_RX_PW_P2));
    printf("%02x ", ReadReg(REG_RX_PW_P3));
    printf("%02x ", ReadReg(REG_RX_PW_P4));
    printf("%02x ", ReadReg(REG_RX_PW_P5));
    printf("%02x", ReadReg(REG_FIFO_STATUS));
    printf("\n");
}
*/

void RF_Init(RF_InitTypeDef* RF_InitStructure)
{
    unsigned char reg;
    RFCON = 0x02;
    RFCON |= _BV(2);

    CLR_CE;
    WriteReg(REG_CONFIG, *(unsigned char*) RF_InitStructure);
    WriteReg(REG_EN_AA, RF_InitStructure->RF_AutoAck_DataPipe);
    WriteReg(REG_EN_RXADDR, RF_InitStructure->RF_RxAddr_DataPipe);
    WriteReg(REG_SETUP_AW, RF_InitStructure->RF_AddrWidth);
    reg = RF_InitStructure->RF_RetryCount
            | (RF_InitStructure->RF_RetryDelay << 4);
    WriteReg(REG_SETUP_RETR, reg);
    WriteReg(REG_RF_CH, RF_InitStructure->RF_Channel);
    reg = (RF_InitStructure->RF_Power << 1)
            | ((RF_InitStructure->RF_DataRate & 1) << 3)
            | ((RF_InitStructure->RF_DataRate >> 1) << 5);
    WriteReg(REG_RF_SETUP, reg);
    WriteData(REG_RX_ADDR_P0, RF_InitStructure->RF_RxAddr_P0, 5);
    WriteData(REG_RX_ADDR_P1, RF_InitStructure->RF_RxAddr_P1, 5);
    WriteReg(REG_RX_ADDR_P2, RF_InitStructure->RF_RxAddr_P2LSB);
    WriteReg(REG_RX_ADDR_P3, RF_InitStructure->RF_RxAddr_P3LSB);
    WriteReg(REG_RX_ADDR_P4, RF_InitStructure->RF_RxAddr_P4LSB);
    WriteReg(REG_RX_ADDR_P5, RF_InitStructure->RF_RxAddr_P5LSB);
    WriteData(REG_TX_ADDR, RF_InitStructure->RF_TxAddr, 5);
    WriteReg(REG_RX_PW_P0, RF_InitStructure->RF_RxPayloadWidth_P0);
    WriteReg(REG_RX_PW_P1, RF_InitStructure->RF_RxPayloadWidth_P1);
    WriteReg(REG_RX_PW_P2, RF_InitStructure->RF_RxPayloadWidth_P2);
    WriteReg(REG_RX_PW_P3, RF_InitStructure->RF_RxPayloadWidth_P3);
    WriteReg(REG_RX_PW_P4, RF_InitStructure->RF_RxPayloadWidth_P4);
    WriteReg(REG_RX_PW_P5, RF_InitStructure->RF_RxPayloadWidth_P5);
    WriteReg(REG_DYNPD, RF_InitStructure->RF_DynamicPayloadWidth_DataPipe);
    reg = RF_InitStructure->RF_TxPayloadNoAckCmd
            | (RF_InitStructure->RF_PayloadWithAck << 1)
            | (RF_InitStructure->RF_DynamicPayloadWidth << 2);
    WriteReg(REG_FEATURE, reg);
}

int RF_TX(void* buf, unsigned char len)
{
    int n;

    RF_FlushTx();
    RF_WriteTxPayload(buf, len);

    RF_SetAsTx();
    SET_CE;

    _delay_us(320);
    CLR_CE;
    _delay_us(320);

    n = 15;
    do {
        n--;
    } while(!(RF_GetStatus() & 0x20) && n);
    RF_ClearIRQ();

    return n;
}

int RF_RX(void* buf, unsigned char len)
{
    CLR_CE;
    RF_SetAsRx();
    SET_CE;
    _delay_us(500);
    while(!(RF_GetStatus() & 0x40))
        ;

    if(RF_GetStatus() & 0x40) {
        RF_ReadRxPayload(len, buf);
    }
    RF_ClearIRQ();

    return 0;
}
