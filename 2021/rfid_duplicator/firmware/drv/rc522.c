#include "rc522.h"
#include "misc.h"

#define MAXRLEN        18
//#define MIN_STRENGTH  228
//#define DEF_FIFO_LENGTH 64  //FIFO size=64byte //  RC522 FIFO长度定义
typedef unsigned char u8_t;

enum {    //                       RC522命令字
    CMD_IDLE = 0x00,    //取消当前命令
    CMD_AUTHENT = 0x0E,    //验证密钥
    CMD_RECEIVE = 0x08,    //接收数据
    CMD_TRANSMIT = 0x04,    //发送数据
    CMD_TRANSCEIVE = 0x0C,    //发送并接收数据
    CMD_RESETPHASE = 0x0F,    //复位
    CMD_CALCCRC = 0x03    //CRC计算
};

enum {    //                   Mifare_One卡片命令字                          */
    PICC_REQIDL = 0x26,    //寻天线区内未进入休眠状态
    PICC_REQALL = 0x52,    //寻天线区内全部卡
    PICC_ANTICOLL1 = 0x93,    //防冲撞
    PICC_ANTICOLL2 = 0x95,    //防冲撞
    PICC_AUTHENT1A = 0x60,    //验证A密钥
    PICC_AUTHENT1B = 0x61,    //验证B密钥
    PICC_READ = 0x30,    //读块
    PICC_WRITE = 0xA0,    //写块
    PICC_DECREMENT = 0xC0,    //扣款
    PICC_INCREMENT = 0xC1,    //充值
    PICC_RESTORE = 0xC2,    //调块数据到缓冲区
    PICC_TRANSFER = 0xB0,    //保存缓冲区中数据
    PICC_HALT = 0x50           //休眠
};

enum {    //                        MF522寄存器定义                           /
    REG_RFU00 = 0x00,
    REG_COMMAND = 0x01,
    REG_COMIEN = 0x02,
    REG_DIVLEN = 0x03,
    REG_COMIRQ = 0x04,
    REG_DIVIRQ = 0x05,
    REG_ERROR = 0x06,
    REG_STATUS1 = 0x07,
    REG_STATUS2 = 0X08,
    REG_FIFODATA = 0X09,
    REG_FIFOLEVEL = 0X0A,
    REG_WATERLEVEL = 0X0B,
    REG_CONTROL = 0X0C,
    REG_BITFRAMING = 0X0D,
    REG_COLL = 0X0E,
    REG_RFU0F = 0X0F,
    REG_RFU10 = 0X10,
    REG_MODE = 0X11,
    REG_TXMODE = 0X12,
    REG_RXMODE = 0X13,
    REG_TXCONTROL = 0X14,
    REG_TXASK = 0X15,
    REG_TXSEL = 0X16,
    REG_RXSEL = 0X17,
    REG_RXTHRESHOLD = 0X18,
    REG_DEMOD = 0X19,
    REG_RFU1A = 0X1A,
    REG_RFU1B = 0X1B,
    REG_MIFARE = 0X1C,
    REG_RFU1D = 0X1D,
    REG_RFU1E = 0X1E,
    REG_SERIALSPEED = 0X1F,
    REG_RFU20 = 0X20,
    REG_CRCRESULTM = 0X21,
    REG_CRCRESULTL = 0X22,
    REG_RFU23 = 0X23,
    REG_MODWIDTH = 0X24,
    REG_RFU25 = 0X25,
    REG_RFCFG = 0X26,
    REG_GSN = 0X27,
    REG_CWGSCFG = 0X28,
    REG_MODGSCFG = 0X29,
    REG_TMODE = 0X2A,
    REG_TPRESCALER = 0X2B,
    REG_TRELOADH = 0X2C,
    REG_TRELOADL = 0X2D,
    REG_TCOUNTERVALUEH = 0X2E,
    REG_TCOUNTERVALUEL = 0X2F,
    REG_RFU30 = 0X30,
    REG_TESTSEL1 = 0X31,
    REG_TESTSEL2 = 0X32,
    REG_TESTPINEN = 0X33,
    REG_TESTPINVALUE = 0X34,
    REG_TESTBUS = 0X35,
    REG_AUTOTEST = 0X36,
    REG_VERSION = 0X37,
    REG_ANALOGTEST = 0X38,
    REG_TESTDAC1 = 0X39,
    REG_TESTDAC2 = 0X3A,
    REG_TESTADC = 0X3B,
    REG_RFU3C = 0X3C,
    REG_RFU3D = 0X3D,
    REG_RFU3E = 0X3E,
    REG_RFU3F = 0X3F
};

static void CalulateCRC(u8_t* pIndata, u8_t len, u8_t* pOutData);

static rc522_cfg_t g;
void RC522_Init(rc522_cfg_t* cfg)
{
    g = *cfg;
    RC522_Reset();
}

//功    能：读RC522寄存器
static u8_t read_reg(u8_t addr)
{
    u8_t ret;
    g.cs_f(0);
    addr = ((addr << 1) & 0x7E) | 0x80;
    g.writebyte_f(addr);
    ret = g.writebyte_f(0xff);
    g.cs_f(1);
    return ret;
}

static void write_reg(u8_t addr, u8_t val)
{
    g.cs_f(0);
    addr = ((addr << 1) & 0x7E);
    g.writebyte_f(addr);
    g.writebyte_f(val);
    g.cs_f(1);
}

//功    能：置RC522寄存器位
static void set_regbit(u8_t reg, u8_t mask)
{
    char tmp = 0x0;
    tmp = read_reg(reg) | mask;
    write_reg(reg, tmp | mask);    // set bit mask
}

//功    能：清RC522寄存器位
static void clr_regbit(u8_t reg, u8_t mask)
{
    char tmp = 0x0;
    tmp = read_reg(reg) & (~mask);
    write_reg(reg, tmp);    // clear bit mask
}

//功    能：复位RC522 返    回: 成功返回MI_OK
char RC522_Reset()
{
//    RF_POWER_ON;
    g.reset_f(1);
    g.delay_us_f(10);
    g.reset_f(0);
    g.delay_us_f(10);
    g.reset_f(1);
    g.delay_us_f(10);
    write_reg(REG_COMMAND, CMD_RESETPHASE);
    g.delay_us_f(10);
    write_reg(REG_MODE, 0X3D);
    write_reg(REG_TRELOADL, 30);
    write_reg(REG_TRELOADH, 0);
    write_reg(REG_TMODE, 0X8D);
    write_reg(REG_TPRESCALER, 0X3E);
//  WriteRawRC(TxASKReg,0x40)            ; // FOR DEBUG AND TEST
    return MI_OK;
}

//开启天线发射 //每次启动或关闭天险发射之间应至少有1ms的间隔
void RC522_AntennaOn()
{
    u8_t i;
    write_reg(REG_TXASK, 0X40);
    g.delay_us_f(10);
    i = read_reg(REG_TXCONTROL);
    if(!(i & 0x03))
        set_regbit(REG_TXCONTROL, 0X03);
    i = read_reg(REG_TXASK);
}

//开启天线发射 //每次启动或关闭天险发射之间应至少有1ms的间隔
void RC522_AntennaTestOn()
{
    g.reset_f(1);
    g.delay_us_f(15);    // 2010.10.09 ???? FOR DEBUG
//  g.delay_us_f(5)                        ; // 2010.10.09 ???? FOR DEBUG
    write_reg(REG_TXCONTROL, 0X02);
    /* g.delay_us_f(1)                         ;
     SetBitMask(REG_TxControl, 0x03);// FOR DEBUG */
}

//关闭天线发射
void RC522_AntennaOff()
{
    clr_regbit(REG_TXCONTROL, 0X03);
}

//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字     pInData[IN]:通过RC522发送到卡片的数据
//  InLenByte[IN]:发送数据的字节长度     pOutData[OUT]:接收到的卡片返回数据
//  *pOutLenBit[OUT]:返回数据的位长度
static char RC522_ComMF522(u8_t cmd, u8_t* in_data, u8_t in_len, u8_t* out_data,
    unsigned int* out_len)
{
    char status = MI_ERR;
    u8_t irqEn = 0x00;
    u8_t waitFor = 0x00;
    u8_t lastBits;
    u8_t n;
    unsigned int i;
    switch(cmd) {
        case CMD_AUTHENT:
            irqEn = 0x12;
            waitFor = 0x10;
            break;
        case CMD_TRANSCEIVE:
            irqEn = 0x77;
            waitFor = 0x30;
            break;
        default:
            break;
    }
    write_reg(REG_COMIEN, irqEn | 0x80);    //
    clr_regbit(REG_COMIRQ, 0x80);
    write_reg(REG_COMMAND, CMD_IDLE);
    set_regbit(REG_FIFOLEVEL, 0x80);    // 清空FIFO
    for(i = 0; i < in_len; i++)
        write_reg(REG_FIFODATA, in_data[i]);    // 数据写入FIFO
    write_reg(REG_COMMAND, cmd);    // 命令写入命令寄存器
    if(cmd == CMD_TRANSCEIVE)
        set_regbit(REG_BITFRAMING, 0X80);    // 开始发送
    i = 6000;    //根据时钟频率调整，操作M1卡最大等待时间25ms
    do {
        n = read_reg(REG_COMIRQ);
        i--;
    } while((i != 0) && !(n & 0x01) && !(n & waitFor));
    clr_regbit(REG_BITFRAMING, 0X80);
    if(i != 0) {
        if(!(read_reg(REG_ERROR) & 0x1B)) {
            status = MI_OK;
            if(n & irqEn & 0x01)
                status = MI_NOTAGERR;
            if(cmd == CMD_TRANSCEIVE) {
                n = read_reg(REG_FIFOLEVEL);
                lastBits = read_reg(REG_CONTROL) & 0X07;
                if(lastBits)
                    *out_len = (n - 1) * 8 + lastBits;
                else
                    *out_len = n * 8;
                if(n == 0)
                    n = 1;
                if(n > MAXRLEN)
                    n = MAXRLEN;
                for(i = 0; i < n; i++)
                    out_data[i] = read_reg(REG_FIFODATA);
            }
        }
        else
            status = MI_ERR;
    }
    set_regbit(REG_CONTROL, 0X80);    // STOP TIMER NOW
    write_reg(REG_COMMAND, CMD_IDLE);
    return status;
}

//功    能：寻卡                                                    /
//参数说明: req_code[IN]:寻卡方式                                   /
//                0x52 = 寻感应区内所有符合14443A标准的卡           /
//                0x26 = 寻未进入休眠状态的卡                       /
//          pTagType[OUT]：卡片类型代码                             /
//                0x4400 = Mifare_UltraLight                        /
//                0x0400 = Mifare_One(S50)                          /
//                0x0200 = Mifare_One(S70)                          /
//                0x0800 = Mifare_Pro(X)                            /
//                0x4403 = Mifare_DESFire                           /
//返    回: 成功返回MI_OK                                           /
char RC522_Request(u8_t req_code, void* pTagType)
{
    char status;
    unsigned int unLen;
    u8_t buf[MAXRLEN];

    clr_regbit(REG_STATUS2, 0X08);
    write_reg(REG_BITFRAMING, 0X07);
    set_regbit(REG_TXCONTROL, 0X03);

    buf[0] = req_code;

    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 1, buf, &unLen);
    if((status == MI_OK) && (unLen == 0x10)) {
        *(u8_t*)pTagType = buf[0];
        *(u8_t*)(pTagType + 1) = buf[1];
    }
    else
        status = MI_ERR;
    return status;
}

//功    能：防冲撞                                                  /
//参数说明: pSnr[OUT]:卡片序列号，4字节                             /
//返    回: 成功返回MI_OK                                           /
char RC522_Anticoll(void* pSnr)
{
    char status;
    u8_t i, snr_check = 0;
    unsigned int unLen;
    u8_t buf[MAXRLEN];

    clr_regbit(REG_STATUS2, 0X08);
    write_reg(REG_BITFRAMING, 0X00);
    clr_regbit(REG_COLL, 0X80);

    buf[0] = PICC_ANTICOLL1;
    buf[1] = 0x20;

    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 2, buf, &unLen);

    if(status == MI_OK) {
        for(i = 0; i < 4; i++) {
            *(u8_t*)(pSnr + i) = buf[i];
            snr_check ^= buf[i];
        }
        if(snr_check != buf[i]) {
            status = MI_ERR;
        }
    }

    set_regbit(REG_COLL, 0X80);
    return status;
}

//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
char RC522_Select(void* pSnr)
{
    char status;
    u8_t i;
    unsigned int unLen;
    u8_t buf[MAXRLEN];

    buf[0] = PICC_ANTICOLL1;
    buf[1] = 0x70;
    buf[6] = 0;
    for(i = 0; i < 4; i++) {
        buf[i + 2] = *(u8_t*)(pSnr + i);
        buf[6] ^= *(u8_t*)(pSnr + i);
    }
    CalulateCRC(buf, 7, &buf[7]);

    clr_regbit(REG_STATUS2, 0X08);

    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 9, buf, &unLen);

    if((status == MI_OK) && (unLen == 0x18)) {
        status = MI_OK;
    }
    else {
        status = MI_ERR;
    }

    return status;
}

//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式 0x60 = 验证A密钥  0x61 = 验证B密钥
//          addr[IN]：块地址   pKey[IN]：密码  pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
char RC522_AuthState(u8_t auth_mode, u8_t addr, u8_t* pKey, void* pSnr)
{
    char status;
    unsigned int unLen;
    u8_t i, buf[MAXRLEN];

    buf[0] = auth_mode;
    buf[1] = addr;
    for(i = 0; i < 6; i++) {
        buf[i + 2] = *(pKey + i);
    }
    for(i = 0; i < 6; i++) {
        buf[i + 8] = *(u8_t*)(pSnr + i);
    }

    status = RC522_ComMF522(CMD_AUTHENT, buf, 12, buf, &unLen);
    if((status != MI_OK) || (!(read_reg(REG_STATUS2) & 0X08))) {
        status = MI_ERR;
    }

    return status;
}

//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
char RC522_Read(u8_t addr, u8_t* pData)
{
    char status;
    unsigned int unLen;
    u8_t i, buf[MAXRLEN];

    buf[0] = PICC_READ;
    buf[1] = addr;
    CalulateCRC(buf, 2, &buf[2]);
    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 4, buf, &unLen);
    if((status == MI_OK) && (unLen == 0x90)) {
        for(i = 0; i < 16; i++)
            *(pData + i) = buf[i];
    }
    else
        status = MI_ERR;
    return status;
}

/*
 //功    能：读取M1卡一块数据
 //参数说明: addr[IN]：块地址          pData[OUT]：读出的数据，16字节
 //返    回: 成功返回MI_OK
 static char ReadBlock(u8_t Block, u8_t* Buf)
 {
 char result;
 result = RC522_AuthState(0x60, Block, Password_Buffer, UID);
 if(result != MI_OK)
 return result;
 result = RC522_Read(Block, Buf);
 //  return result; // 2011.01.03

 if(result != MI_OK)
 return result;
 if(Block != 0x00 && des_on) {
 Des_Decrypt((char*)Buf, KK, (char*)Buf);
 Des_Decrypt((char*)&Buf[8], KK, (char*)&Buf[8]);
 }
 return MI_OK;    // wait for debug 2019.1.21
 }
 */

//功    能：写数据到M1卡一块fm17520
//参数说明: addr[IN]：块地址        pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
char RC522_Write(u8_t addr, u8_t* pData)
{
    char status;
    unsigned int unLen;
    u8_t i, buf[MAXRLEN];

    if(addr == 0) {
        RC522_Halt();
        set_regbit(REG_BITFRAMING, 0x07);
        buf[0] = 0x40;
        status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 1, buf, &unLen);
       xprintf("### %02x %02x\n", status, buf[0]);
        clr_regbit(REG_BITFRAMING, 0x07);
        buf[0] = 0x43;
        status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 1, buf, &unLen);
        xprintf("### %02x %02x\n", status, buf[0]);
    }
    buf[0] = PICC_WRITE;
    buf[1] = addr;
    CalulateCRC(buf, 2, &buf[2]);
    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 4, buf, &unLen);
    if((status != MI_OK) || (unLen != 4) || ((buf[0] & 0x0F) != 0x0A))
        status = MI_ERR;
    if(status == MI_OK) {
        for(i = 0; i < 16; i++)
            buf[i] = *(pData + i);
        CalulateCRC(buf, 16, &buf[16]);
        status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 18, buf, &unLen);
        if((status != MI_OK) || (unLen != 4) || ((buf[0] & 0x0F) != 0x0A))
            status = MI_ERR;
    }
    return status;
}
/*
 //功    能：写数据到M1卡一块
 //参数说明: addr[IN]：块地址 pData[IN]：写入的数据，16字节
 //返    回: 成功返回MI_OK
 static char Write_Block(u8_t Block)
 {
 char result;
 if(des_on) {
 Des_Encrypt((char*)RF_Buffer, KK, (char*)RF_Buffer);    // for debug
 Des_Encrypt((char*)&RF_Buffer[8], KK, (char*)&RF_Buffer[8]);    // for debug
 }
 result = RC522_AuthState(0x60, Block, Password_Buffer, UID);
 if(result != MI_OK)
 return result;
 result = RC522_Write(Block, RF_Buffer);
 return result;
 }
 */

//功    能：扣款和充值
//参数说明: dd_mode[IN]：命令字  0xC0 = 扣款    0xC1 = 充值
//          addr[IN]：钱包地址  pValue[IN]：4字节增(减)值，低位在前
//返    回: 成功返回MI_OK
char RC522_Value(u8_t dd_mode, u8_t addr, u8_t* pValue)
{
    char status;
    unsigned int unLen;
    u8_t i, buf[MAXRLEN];

    buf[0] = dd_mode;
    buf[1] = addr;
    CalulateCRC(buf, 2, &buf[2]);

    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 4, buf, &unLen);

    if((status != MI_OK) || (unLen != 4) || ((buf[0] & 0x0F) != 0x0A)) {
        status = MI_ERR;
    }

    if(status == MI_OK) {
        for(i = 0; i < 16; i++) {
            buf[i] = *(pValue + i);
        }
        CalulateCRC(buf, 4, &buf[4]);
        unLen = 0;
        status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 6, buf, &unLen);
        if(status != MI_ERR) {
            status = MI_OK;
        }
    }

    if(status == MI_OK) {
        buf[0] = PICC_TRANSFER;
        buf[1] = addr;
        CalulateCRC(buf, 2, &buf[2]);

        status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 4, buf, &unLen);

        if((status != MI_OK) || (unLen != 4) || ((buf[0] & 0x0F) != 0x0A)) {
            status = MI_ERR;
        }
    }
    return status;
}

//功    能：备份钱包
//参数说明: sourceaddr[IN]：源地址           goaladdr[IN]：目标地址
//返    回: 成功返回MI_OK
char RC522_BakValue(u8_t sourceaddr, u8_t goaladdr)
{
    char status;
    unsigned int unLen;
    u8_t buf[MAXRLEN];

    buf[0] = PICC_RESTORE;
    buf[1] = sourceaddr;
    CalulateCRC(buf, 2, &buf[2]);

    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 4, buf, &unLen);

    if((status != MI_OK) || (unLen != 4) || ((buf[0] & 0x0F) != 0x0A)) {
        status = MI_ERR;
    }

    if(status == MI_OK) {
        buf[0] = 0;
        buf[1] = 0;
        buf[2] = 0;
        buf[3] = 0;
        CalulateCRC(buf, 4, &buf[4]);

        status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 6, buf, &unLen);
        if(status != MI_ERR) {
            status = MI_OK;
        }
    }

    if(status != MI_OK) {
        return MI_ERR;
    }

    buf[0] = PICC_TRANSFER;
    buf[1] = goaladdr;

    CalulateCRC(buf, 2, &buf[2]);

    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 4, buf, &unLen);

    if((status != MI_OK) || (unLen != 4) || ((buf[0] & 0x0F) != 0x0A)) {
        status = MI_ERR;
    }

    return status;
}

//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
char RC522_Halt(void)
{
    char status;
    unsigned int unLen;
    u8_t buf[MAXRLEN];

    buf[0] = PICC_HALT;
    buf[1] = 0;
    CalulateCRC(buf, 2, &buf[2]);

    status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 4, buf, &unLen);
    return status;
//    return MI_OK;
}

/*
 //功    能：命令卡片进入休眠状态
 //返    回: 成功返回MI_OK
 char MIF_Halt(void)
 {
 char status;
 unsigned int unLen;
 u8_t buf[MAXRLEN];

 buf[0] = PICC_HALT;
 buf[1] = 0;
 CalulateCRC(buf, 2, &buf[2]);

 status = RC522_ComMF522(CMD_TRANSCEIVE, buf, 4, buf, &unLen);
 return status;
 //    return MI_OK;
 }
 */

//用MF522计算CRC16函数
static void CalulateCRC(u8_t* pIndata, u8_t len, u8_t* pOutData)
{
    u8_t i, n;
    clr_regbit(REG_DIVIRQ, 0x04);
    write_reg(REG_COMMAND, CMD_IDLE);
    set_regbit(REG_FIFOLEVEL, 0X80);
    for(i = 0; i < len; i++) {
        write_reg(REG_FIFODATA, *(pIndata + i));
    }
    write_reg(REG_COMMAND, CMD_CALCCRC);
    i = 0xFF;
    do {
        n = read_reg(REG_DIVIRQ);
        i--;
    } while((i != 0) && !(n & 0x04));
    pOutData[0] = read_reg(REG_CRCRESULTL);
    pOutData[1] = read_reg(REG_CRCRESULTM);
}

