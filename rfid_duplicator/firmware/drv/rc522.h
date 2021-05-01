#ifndef __RC522_H
#define __RC522_H

typedef struct {
    unsigned char (*writebyte_f)(unsigned char);
    void (*cs_f)(int cs);
    void (*reset_f)(int reset);
    void (*delay_us_f)(volatile int nus);
} rc522_cfg_t;

enum {    //                    RC522通讯返回错误代码                         /
    MI_ERR = 0xFE,
    MI_OK = 0x0,
    MI_CHK_OK = 0x0,
    MI_CRC_ZERO = 0x0,
    MI_CRC_NOTZERO = 0x1,
    MI_NOTAGERR = 0xFF,
    MI_CHK_FAILED = 0xFF,
    MI_CRCERR = 0xFE,
    MI_CHK_COMPERR = 0xFE,
    MI_EMPTY = 0xFD,
    MI_AUTHERR = 0xFC,
    MI_PARITYERR = 0xFB,
    MI_CODEERR = 0xFA,
    MI_SERNRERR = 0xF8,
    MI_KEYERR = 0xF7,
    MI_NOTAUTHERR = 0xF6,
    MI_BITCOUNTERR = 0xF5,
    MI_BYTECOUNTERR = 0xF4,
    MI_IDLE = 0xF3,
    MI_TRANSERR = 0xF2,
    MI_WRITEERR = 0xF1,
    MI_INCRERR = 0xF0,
    MI_DECRERR = 0xEF,
    MI_READERR = 0xEE,
    MI_OVFLERR = 0xED,
    MI_POLLING = 0xEC,
    MI_FRAMINGERR = 0xEB,
    MI_ACCESSERR = 0xEA,
    MI_UNKNOWN_COMMAND = 0xE9,
    MI_COLLERR = 0xE8,
    MI_RESETERR = 0xE7,
    MI_INITERR = 0xE7,
    MI_INTERFACEERR = 0xE7,
    MI_ACCESSTIMEOUT = 0xE5,
    MI_NOBITWISEANTICOLL = 0xE4,
    MI_QUIT = 0xE2,
    MI_RECBUF_OVERFLOW = 0xCF,
    MI_SENDBYTENR = 0xCE,
    MI_SENDBUF_OVERFLOW = 0xCC,
    MI_BAUDRATE_NOT_SUPPORTED = 0xCB,
    MI_SAME_BAUDRATE_REQUIRED = 0xCA,
    MI_WRONG_PARAMETER_VALUE = 0xC5,
    MI_BREAK = 0x9E,
    MI_NY_IMPLEMENTED = 0x9D,
    MI_NO_MFRC = 0x9C,
    MI_MFRC_NOTAUTH = 0x9B,
    MI_WRONG_DES_MODE = 0x9A,
    MI_HOST_AUTH_FAILED = 0x99,
    MI_WRONG_LOAD_MODE = 0x97,
    MI_WRONG_DESKEY = 0x96,
    MI_MKLOAD_FAILED = 0x95,
    MI_FIFOERR = 0x94,
    MI_WRONG_ADDR = 0x93,
    MI_DESKEYLOAD_FAILED = 0x92,
    MI_WRONG_SEL_CNT = 0x8F,
    MI_RC531_WRONG_READVALUE = 0x8E,
    MI_WRONG_TEST_MODE = 0x8C,
    MI_TEST_FAILED = 0x8B,
    MI_TOC_ERROR = 0x8A,
    MI_COMM_ABORT = 0x89,
    MI_INVALID_BASE = 0x88,
    MI_MFRC_RESET = 0x87,
    MI_WRONG_VALUE = 0x86,
    MI_VALERR = 0x85
};

void RC522_Init(rc522_cfg_t* cfg);

char RC522_Halt(void);
char RC522_Reset(void);
void RC522_AntennaOn(void);
void RC522_AntennaOff(void);
char RC522_Request(unsigned char req_code, void* pTagType);
char RC522_Anticoll(void* pSnr);
char RC522_Select(void* pSnr);
char RC522_AuthState(unsigned char auth_mode, unsigned char addr,
    unsigned char* pKey, void* pSnr);
char RC522_Read(unsigned char addr, unsigned char* pData);
char RC522_Write(unsigned char addr, unsigned char* pData);
char RC522_Value(unsigned char dd_mode, unsigned char addr,
    unsigned char* pValue);
char RC522_BakValue(unsigned char sourceaddr, unsigned char goaladdr);
//char RC522_ComMF522(unsigned char Command, unsigned char* pInData, unsigned char InLenByte, unsigned char* pOutData, unsigned int* pOutLenBit);
//char ReadBlock(unsigned char Block, unsigned char* Buf);
//char Write_Block(unsigned char Block);
void RC522_AntennaTestOn(void);

#endif
