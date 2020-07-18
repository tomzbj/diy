#include <string.h>
#include <stdio.h>
#include "zmb.h"

#define MIN_ASCII_FRAME_SIZE 17       // header 5 bytes, msg 8bytes, trailer 4 bytes
#define MIN_RTU_FRAME_SIZE 8       // header 2 bytes, msg 4 bytes, crc 2 bytes
#define NUM_OF_HOLDING_REGS 0

#define GetFrameLRC(rawframe, size) AsciiToByte(&rawframe[size - 4])
#define GetFrameCRC(rawframe, size) (rawframe[size - 2] + rawframe[size - 1] * 256)
#define GetAsciiFrameAddr(rawframe) AsciiToByte(&rawframe[1])
#define GetRtuFrameAddr(rawframe) (rawframe[0])
// 把两位ascii 16进制数转换成1个字节
#define AsciiToWord(x) (AsciiToByte(x) * 256UL + AsciiToByte(x + 2))

enum {
    ZMB_TYPE_ASCII, ZMB_TYPE_RTU
};

typedef struct {
    unsigned char addr;         // Device Address, 1..127
    unsigned char func;         // Function Code, 03/04/06
    unsigned short reg;         // Start address of Register(s)
    unsigned short n_regs;         // Number of Register(s)
    unsigned short data;        // Data (16bits), only for query
    unsigned char trl;          // Trailer
    unsigned char type;         // MODBUS TYPE
} frame_t;

static struct {
    unsigned char buf[128];   // General buffer
    void (*write_f)(const void* msg, int size);
    unsigned short* regs;
    int n_regs;
    unsigned char addr;
} g;

#define __tolower(x) (((x) >= 'A' && (x) <= 'Z')? ((x) - 'A' + 'a') : (x))

// @brief 把2个ASCII字符转换成1字节.
static unsigned char AsciiToByte(const unsigned char str[2])
{
    unsigned char ret = 0, s0, s1;
    s0 = __tolower((unsigned char )str[0]);
    s1 = __tolower((unsigned char )str[1]);

    if(s0 >= '0' && s0 <= '9')
        ret = (s0 - '0') * 16;
    else if(s0 >= 'a' && s0 <= 'f')
        ret = (s0 - 'a' + 10) * 16;

    if(s1 >= '0' && s1 <= '9')
        ret |= s1 - '0';
    else if(s1 >= 'a' && s1 <= 'f')
        ret |= s1 - 'a' + 10;

    return ret;
}

// @brief 计算LRC校验值.
static unsigned char CalcLRC(const unsigned char* rawframe, int size)
{
    unsigned char i, lrc = 0;
    size -= 4;  // Truncate LRC & TRL
    for(i = 1; i < size; i += 2)
        lrc += AsciiToByte(&rawframe[i]);
    return (~lrc + 1);
}

// @brief 计算CRC16校验值, 多项式为0xa001.
static unsigned short CalcCRC(const unsigned char* msg, int size)
{
    size -= 2;      // truncate received crc
    unsigned short crc = 0xffff;
    for(int i = 0; i < size; i++) {
        crc ^= msg[i];
        for(int j = 0; j < 8; j++) {
            int lsb = crc % 2;
            crc >>= 1;
            if(lsb)
                crc ^= 0xa001;
        }
    }
    return crc;
}

// @brief 发送回应.
static void Respond(frame_t* pf, const unsigned char* data)
{
    unsigned short crc;
    unsigned char lrc;
    int size;
    if(pf->type == ZMB_TYPE_RTU) {
        if(pf->func == 0x04) {
            sprintf((char*)g.buf, "%c%c%c", pf->addr, pf->func,
                pf->n_regs * 2);
            memcpy(g.buf + 3, data, pf->n_regs * 2);
            crc = CalcCRC(g.buf, 3 + pf->n_regs * 2 + 2);
            sprintf((char*)g.buf + 3 + pf->n_regs * 2, "%c%c", crc, crc >> 8);
            size = 3 + pf->n_regs * 2 + 2;
        }
    }
    else {
        if(pf->func == 0x04) {
            sprintf((char*)g.buf, ":%02x%02x%02x%s", pf->addr, pf->func,
                pf->n_regs * 2, data);
            lrc = CalcLRC(g.buf, 7 + pf->n_regs * 4 + 4);
            size = sprintf((char*)g.buf, ":%02x%02x%02x%s%02x\r\n", pf->addr,
                pf->func, pf->n_regs * 2, data, lrc);
        }
    }
    g.write_f(g.buf, size);
}

// @brief 发送错误回应.
static void RespondError(frame_t* pf, int errcode)
{
    int size;
    unsigned short crc;
    unsigned char lrc;
    pf->func += 0x80;
    if(pf->type == ZMB_TYPE_RTU) {
        sprintf((char*)g.buf, "%c%c%c", pf->addr, pf->func, errcode);
        crc = CalcCRC(g.buf, 5); // full size of respond
        size = sprintf((char*)g.buf, "%c%c%c%c%c", pf->addr, pf->func, errcode,
            crc, crc >> 8);
    }
    else {
        sprintf((char*)g.buf, ":%02x%02x%02x", pf->addr, pf->func, errcode);
        lrc = CalcLRC(g.buf, 7 + 4); // full size of respond
        size = sprintf((char*)g.buf, ":%02x%02x%02x%02x\r\n", pf->addr,
            pf->func, errcode, lrc);
    }
    g.write_f(g.buf, size);
}

// @retval 若传入帧完整, 返回ZMB_CHECK_OK, 否则返回相应的错误代码.
int zmb_integrity_check(const void* msg, int size)
{
    unsigned char* p = (unsigned char*)msg;
    if(p[0] == ':') {
        if(size < MIN_ASCII_FRAME_SIZE)
            return ZMB_CHECK_FRAME_TOO_SHORT;
        if(CalcLRC(p, size) != GetFrameLRC(p, size))
            return ZMB_CHECK_LRC_CHECK_FAILED;
        if(GetAsciiFrameAddr(p) != g.addr)
            return ZMB_CHECK_ADDRESS_MISMATCH;
    }
    else {
        if(size < MIN_RTU_FRAME_SIZE)
            return ZMB_CHECK_FRAME_TOO_SHORT;
        if(CalcCRC(p, size) != GetFrameCRC(p, size))
            return ZMB_CHECK_CRC_CHECK_FAILED;
        if(GetRtuFrameAddr(p) != g.addr)
            return ZMB_CHECK_ADDRESS_MISMATCH;
    }
    return ZMB_CHECK_OK;
}

// @brief 进一步检查传入帧合法性.
static int ValidityCheck(frame_t* pf)
{
    switch(pf->func) {
        case 0x04:
            if(pf->reg + pf->n_regs > g.n_regs)
                return ZMB_EXCEPTION_ILLEGAL_DATA_ADDRESS;
            break;
        default:
            return ZMB_EXCEPTION_ILLEGAL_FUNCTION;
    }
    return ZMB_EXCEPTION_OK;
}

// @brief 帧解析
static void Parse(const unsigned char* msg, frame_t* pf)
{
    if(msg[0] == ':') {
        pf->type = ZMB_TYPE_ASCII;
        pf->addr = AsciiToByte(&msg[1]);
        pf->func = AsciiToByte(&msg[3]);
        pf->reg = AsciiToWord(&msg[5]);
        if(pf->func == 0x06) {
            pf->data = AsciiToWord(&msg[9]);
            pf->n_regs = 1;
        }
        else
            // only func code 04 available
            pf->n_regs = AsciiToWord(&msg[9]);
    }
    else {
        pf->type = ZMB_TYPE_RTU;
        pf->addr = msg[0];
        pf->func = msg[1];
        pf->reg = msg[2] * 256 + msg[3];
        pf->n_regs = 0;
        if(pf->func == 0x04) {
            pf->n_regs = msg[4] * 256 + msg[5];
        }
    }
}

// @brief Modbus 写寄存器
void zmb_write_reg(int regid, int val)
{
    g.regs[regid] = val;
}

unsigned short zmb_read_reg(int regid)
{
    return g.regs[regid];
}

// @brief Modbus 解析入口.
void zmb_parse(const void* msg, int size, void (*write_f)(const void*, int))
{
    unsigned char buf[128];
    frame_t f;
    g.write_f = write_f;

    Parse(msg, &f);

    int ret = ValidityCheck(&f);
    if(ret != ZMB_EXCEPTION_OK) {
        RespondError(&f, ret);
        return;
    }
    switch(f.func) {
        case 0x4:           // Read Input Register
            for(int i = 0; i < f.n_regs; i++) {
                unsigned short reg = g.regs[f.reg + i];
                if(f.type == ZMB_TYPE_RTU)
                    sprintf((char*)(buf + i * 2), "%c%c", reg >> 8, reg);
                else
                    sprintf((char*)(buf + i * 4), "%04x", reg);
            }
            Respond(&f, buf);
            break;
    }
}

void zmb_init(unsigned short* regs, int n_regs, unsigned char addr)
{
    g.regs = regs;
    g.n_regs = n_regs;
    g.addr = addr;
}
