#include "spiflash.h" 

enum {
    CMD_READ = 0x03, CMD_FAST_READ = 0x0b, CMD_ERASE4K = 0x20,
    CMD_ERASE32K = 0x52, CMD_ERASE64K = 0xd8, CMD_ERASECHIP = 0x60,
    CMD_BYTE_PROGRAM = 0x02, CMD_AAI_WORD_PROGRAM = 0xad,
    CMD_READ_STATUS = 0x05, CMD_EN_WRITE_STATUS = 0x50,
    CMD_WRITE_STATUS = 0x01, CMD_WRITE_ENABLE = 0x06, CMD_WRITE_DISABLE = 0x04,
    CMD_READ_ID = 0x90, CMD_JEDEC_ID = 0x9f, CMD_POWERDOWN = 0xb9,
    CMD_RELEASE_POWERDOWN = 0xab
};

#define ReadByte() g.writebyte_f(0xff)
#define WriteEnable() WriteCmd(CMD_WRITE_ENABLE)
#define WriteDisable() WriteCmd(CMD_WRITE_DISABLE)
//#define WaitBusy() ({unsigned char _st; while((_st = SPIFLASH_ReadStatus()) & 0x1) {_delay_ms_loop(1); uprintf(USART1, "%02x\n", _st);}})
#define WaitBusy() ({while(SPIFLASH_ReadStatus() & 0x1); })

static spiflash_cfg_t g;

static void WriteCmd(unsigned char cmd);

#include "misc.h"
void SPIFLASH_Init(spiflash_cfg_t* cfg)
{
    g = *cfg;
//    unsigned char ret;
//    _delay_ms_loop(100);

#if 1
    g.cs_f(0);
    g.writebyte_f(CMD_EN_WRITE_STATUS);
    g.cs_f(1);
    ( {  for(volatile int n = 1000UL; n; n--);});
    g.cs_f(0);
    g.writebyte_f(CMD_WRITE_STATUS);
    g.writebyte_f(0x00);
    g.cs_f(1);
    WaitBusy();
#endif
}

static void WriteCmd(unsigned char cmd)
{
    g.cs_f(0);
    g.writebyte_f(cmd);
    g.cs_f(1);
}

unsigned char SPIFLASH_ReadStatus(void)
{
    unsigned char ret;
    g.cs_f(0);
    g.writebyte_f(CMD_READ_STATUS);
    ret = ReadByte();
    g.cs_f(1);
    return ret;
}

unsigned long SPIFLASH_ReadJedecID(void)
{
    unsigned long ret;

    g.cs_f(0);
    g.writebyte_f(CMD_JEDEC_ID);
    ret = ReadByte();
    ret <<= 8;
    ret |= ReadByte();
    ret <<= 8;
    ret |= ReadByte();
    g.cs_f(1);
    return ret;
}

int SPIFLASH_FastRead(unsigned long addr, int num, void* buf)
{
    WaitBusy();
    g.cs_f(0);
    g.writebyte_f(CMD_FAST_READ);
    g.writebyte_f(addr >> 16);
    g.writebyte_f(addr >> 8);
    g.writebyte_f(addr);
    g.writebyte_f(0xff);

    g.fastread_f(buf, num);
    g.cs_f(1);
    WaitBusy();
    return 0;
}

int SPIFLASH_Read(unsigned long addr, int num, void* buf)
{
    WaitBusy();
    g.cs_f(0);
    g.writebyte_f(CMD_FAST_READ);
    g.writebyte_f(addr >> 16);
    g.writebyte_f(addr >> 8);
    g.writebyte_f(addr);
    g.writebyte_f(0xff);

    for(int i = 0; i < num; i++) {
        *(unsigned char*)buf = ReadByte();
        buf++;
    }
    g.cs_f(1);
    WaitBusy();
    return 0;
}

int SPIFLASH_Write(unsigned long addr, int num, const void* buf)
{
    WaitBusy();
    while(num > 0) {
        WriteEnable();
        g.cs_f(0);
        g.writebyte_f(0x02);
        g.writebyte_f(addr >> 16);
        g.writebyte_f(addr >> 8);
        g.writebyte_f(addr);
        do {
            g.writebyte_f(*(unsigned char*)buf);
            addr++;
            buf++;
            num--;
        } while((addr & 0xff) && (num > 0));
        g.cs_f(1);
        WaitBusy();
//        _delay_ms_loop(3);
        WriteDisable();
    }
    return 0;
}

void SPIFLASH_SectorErase(unsigned long addr)
{
    WaitBusy();
    WriteEnable();
    g.cs_f(0);
    g.writebyte_f(CMD_ERASE4K);
    g.writebyte_f(addr >> 16);
    g.writebyte_f(addr >> 8);
    g.writebyte_f(addr);
    g.cs_f(1);
    WriteDisable();
    WaitBusy();
}

int SPIFLASH_Erase(unsigned long addr, int nbytes)
{
    addr &= 0xfffff000;
    do {
        SPIFLASH_SectorErase(addr);
        addr += 4096UL;
        nbytes -= 4096UL;
    } while(nbytes > 0);
    return 0;
}

void SPIFLASH_ChipErase(void)
{
    WaitBusy();
    WriteEnable();
    g.cs_f(0);
    g.writebyte_f(CMD_ERASECHIP);
    g.cs_f(1);
    WriteDisable();
    WaitBusy();
}

void SPIFLASH_EnterLowPowerMode(void)
{
    WaitBusy();
    g.cs_f(0);
    g.writebyte_f(CMD_POWERDOWN);
    g.cs_f(1);
}

void SPIFLASH_LeaveLowPowerMode(void)
{
    g.cs_f(0);
    g.writebyte_f(CMD_RELEASE_POWERDOWN);
    g.cs_f(1);
    _delay_us(10);
}
