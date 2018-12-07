/* Includes ------------------------------------------------------------------*/
#include "vs1003.h"
#include "vs1003_port.h"
//#include "config.h"
#include "misc.h"
#include "usart.h"

#define WriteByte(x) VS1003_WriteByte(x)
#define ReadByte() VS1003_WriteByte(0xff)
#define wait() ({  while(VS1003_GetDREQ() == 0);}) /* 等待空闲 */

enum {
    REG_MODE = 0x0, REG_STATUS = 0x1, REG_BASS = 0x2, REG_CLOCKF = 0x3,
    REG_DECODE_TIME = 0x4, REG_AUDATA = 0x5, REG_WRAM = 0x6, REG_WRAMADDR = 0x7,
    REG_HDAT0 = 0x8, REG_HDAT1 = 0x9, REG_AIADDR = 0xa, REG_VOL = 0xb,
    REG_AICTRL0 = 0xc, REG_AICTRL1 = 0xd, REG_AICTRL2 = 0xe, REG_AICTRL3 = 0xf
};

void VS1003_PowerDown(void)
{
    VS1003_SetRESET(0);
}

static unsigned short ReadReg(unsigned char reg)
{
    unsigned short val;

    wait();
    VS1003_SPI_Config(VS1003_SPI_LS);
    VS1003_SetDCS(1);
    VS1003_SetCCS(0);
    WriteByte(VS_READ_COMMAND);/* 发送VS1003的读命令 */
    WriteByte(reg);
    val = ReadByte();
    val <<= 8;
    val |= ReadByte();
    VS1003_SetCCS(1);
    VS1003_SPI_Config(VS1003_SPI_HS);

    return val;
}

static void WriteReg(unsigned char reg, unsigned short val)
{
    wait();
    VS1003_SPI_Config(VS1003_SPI_LS);
    VS1003_SetDCS(1);
    VS1003_SetCCS(0);
    WriteByte(VS_WRITE_COMMAND);
    WriteByte(reg);
    WriteByte(val >> 8);
    WriteByte(val);
    VS1003_SetCCS(1);
    VS1003_SPI_Config(VS1003_SPI_HS);
}

void VS1003_WriteData(unsigned char *buf, int size)
{
    wait();
    VS1003_SetDCS(0);
    while(size) {
        WriteByte(*buf);
        buf++;
        size--;
    }
    VS1003_SetDCS(1);
    VS1003_SetCCS(1);
}

void VS1003_ResetDecodeTime(void)
{
    WriteReg(REG_DECODE_TIME, 0x0000);
    WriteReg(REG_DECODE_TIME, 0x0000); /* 操作两次 */
}

#if 0
unsigned short VS1003_GetDecodeTime(void)
{
    return ReadReg(REG_DECODE_TIME);
}
#endif

void VS1003_SoftReset(void)
{
    unsigned char retry;

    wait();
    WriteByte(0xff); /* 启动传输 */
    retry = 0;
    while(ReadReg(0) != 0x0804) { /* 软件复位,新模式 */
        WriteReg(REG_MODE, 0x0804); /* 软件复位,新模式 */
        _delay_us(2000); /* 等待至少1.35ms */
        if(retry++ > 100) {
            break;
        }
    }
    wait();
    retry = 0;

    while(ReadReg(REG_CLOCKF) != 0x93e8) {// 设置vs1003的时钟,4倍频 , 禁止临时超频
        WriteReg(REG_CLOCKF, 0x93e8);
        if(retry++ > 100) {
            break;
        }
    }
    retry = 0;

/*    while(ReadReg(REG_AUDATA) != 0XBB81) {
        WriteReg(REG_AUDATA, 0XBB81);
        if(retry++ > 100) {
            break;
        }
    } */

    WriteReg(REG_VOL, 0x0000); /* 音量 */
    VS1003_ResetDecodeTime(); /* 复位解码时间 */
    /* 向vs1003发送4个字节无效数据，用以启动SPI发送 */
    VS1003_SetDCS(0); /* 选中数据传输 */
    WriteByte(0xff);
    WriteByte(0xff);
    WriteByte(0xff);
    WriteByte(0xff);
    VS1003_SetDCS(1); /* 取消数据传输 */
    _delay_ms_loop(20);
}

#if 0
void VS1003_Reset(void)
{
    _delay_ms(20);
    WriteByte(0xff);
    VS1003_SetDCS(1); /* 取消数据传输 */
    VS1003_SetCCS(1); /* 取消数据传输 */

    wait();
    _delay_ms(20);
}
#endif

void VS1003_SineTest(void)
{
    WriteReg(0x0b, 0X2020); /* 设置音量 */
    WriteReg(REG_MODE, 0x0820); /* 进入vs1003的测试模式 */
    wait();

    /* 向vs1003发送正弦测试命令：0x53 0xef 0x6e n 0x00 0x00 0x00 0x00 */
    VS1003_SetDCS(0);/* 选中数据传输 */
    WriteByte(0x53);
    WriteByte(0xef);
    WriteByte(0x6e);
    WriteByte(0x24);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    _delay_ms_loop(100);
    VS1003_SetDCS(1);
    /* 退出正弦测试 */
    VS1003_SetDCS(0);/* 选中数据传输 */
    WriteByte(0x45);
    WriteByte(0x78);
    WriteByte(0x69);
    WriteByte(0x74);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    _delay_ms_loop(100);
    VS1003_SetDCS(1);

    /* 再次进入正弦测试并设置n值为0x44，即将正弦波的频率设置为另外的值 */
    VS1003_SetDCS(0);/* 选中数据传输 */
    WriteByte(0x53);
    WriteByte(0xef);
    WriteByte(0x6e);
    WriteByte(0x44);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    _delay_ms_loop(100);
    VS1003_SetDCS(1);
    /* 退出正弦测试 */
    VS1003_SetDCS(0);/* 选中数据传输 */
    WriteByte(0x45);
    WriteByte(0x78);
    WriteByte(0x69);
    WriteByte(0x74);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    _delay_ms_loop(100);
    VS1003_SetDCS(1);
}

#if 0
void VS1003_RamTest(void)
{
    volatile unsigned short value;

    VS1003_Reset();
    WriteReg(REG_MODE, 0x0820);/* 进入vs1003的测试模式 */
    wait();
    VS1003_SetDCS(0); /* xDCS = 1，选择vs1003的数据接口 */
    WriteByte(0x4d);
    WriteByte(0xea);
    WriteByte(0x6d);
    WriteByte(0x54);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    WriteByte(0x00);
    _delay_ms(50);
    VS1003_SetDCS(1);
    value = ReadReg(REG_HDAT0); /* 如果得到的值为0x807F，则表明完好 */
    uprintf(USART1, "%04x\n", value);
}

void VS1003_SetVol(void)
{
    /* Private variables ---------------------------------------------------------*/
    /* VS1003设置参数 */
    /* 0 , henh.1 , hfreq.2 , lenh.3 , lfreq 5 ,主音量 */
    unsigned char vs1003ram[5] = {0, 0, 0, 0, 250};

    unsigned char i;
    unsigned short bass = 0; /* 暂存音调寄存器值 */
    unsigned short volt = 0; /* 暂存音量值 */
    unsigned char vset = 0; /* 暂存音量值 */

    vset = 255 - vs1003ram[4]; /* 取反得到最大值 */
    volt = vset;
    volt <<= 8;
    volt += vset;/* 得到音量设置后大小 */
    /* 0,henh.1,hfreq.2,lenh.3,lfreq */
    for(i = 0; i < 4; i++) {
        bass <<= 4;
        bass += vs1003ram[i];
    }
    WriteReg(REG_BASS, bass);/* BASS */
    WriteReg(REG_VOL, volt); /* 设音量 */
}
#endif

void VS1003_Init(void)
{
    VS1003_SetCCS(1);
    VS1003_SetDCS(1);
    VS1003_SetRESET(1);
    wait();
    VS1003_SoftReset();
    wait();
}
