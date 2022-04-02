
#include "misc.h"
#include "platform.h"

#define MSG_LEN 128

typedef enum {
    CRC_OK = 0, CRC_MISMATCH = -1
} CRC_Status;

//static struct { USART_TypeDef* USARTx; } g = {USART3};
#define ok() xprintf("ok.\n")

static unsigned short CRC16_Calc(const unsigned char* msg, int size)
{
    unsigned short crc = 0xffff;
    for(int i = 0; i < size; i++) {
        crc ^= msg[i];
        for(int j = 0; j < 8; j++) {
            bool lsb = crc % 2;
            crc >>= 1;
            if(lsb)
                crc ^= 0xa001;
        }
    }
    return crc;
}

CRC_Status CRC16_CheckIntegrity(const unsigned char* msg, int size)
{
    unsigned short crc, crc2;
    crc = CRC16_Calc(msg, size - 2);
    memcpy(&crc2, &msg[size - 2], 2);
    if(crc == crc2)
        return CRC_OK;
    else
        return CRC_MISMATCH;
}

static void ParseRD(char* const tokens[], int count)
{
//    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "zt_ntasks") == 0) {
        xprintf("%d\n", zt_get_num_tasks());
    }
//    else if(strcasecmp(tokens[1], "rfid") == 0) { RFID_Read(); }
//    else if(strcasecmp(tokens[1], "rfid_view") == 0) { RFID_View(); }
    else if(strcasecmp(tokens[1], "rfid_readid") == 0) {
        xprintf("%lX", RFID_ReadID());
    }
}

static void ParseWR(char* const tokens[], int count)
{
//    DataConfig_t* pdc = DC_Get();

    if(0) {
    }
    else if(strcasecmp(tokens[1], "zt_start") == 0) {
        int tid = strtol(tokens[2], NULL, 10);
        zt_start(tid);
        ok();
    }
    else if(strcasecmp(tokens[1], "zt_stop") == 0) {
        int tid = strtol(tokens[2], NULL, 10);
        zt_stop(tid);
        ok();
    }
}

static void Parse(char* const tokens[], int count)
{
    if(strcasecmp(tokens[0], "TEST") == 0) {
        xprintf("TEST OK\n");
    }
    else if(strcasecmp(tokens[0], "REBOOT") == 0) {
        xprintf("Done.\n");
        NVIC_SystemReset();
    }
}

void CLI_Parse(const unsigned char* msg, int size)    //, int type)
{
    char* tokens[8], * token;
    char seps[] = "? ,#", string[MSG_LEN];
    int len, count = 0;

    bzero(string, MSG_LEN);
    strncpy(string, (const char*)msg, size);

    len = strlen(string);
    while(string[len - 1] == '\n' || string[len - 1] == '\r') {
        string[len - 1] = '\0';
        len--;
    }

    token = strtok(string, seps);
    while(token != NULL) {
        tokens[count] = token;
        token = strtok(NULL, seps);    // Get next token:
        count++;
        if(count >= 8)
            break;
    }
    if(count == 0)
        return;

    if(strcasecmp(tokens[0], "RD") == 0) {
        ParseRD(tokens, count);
    }
    else if(strcasecmp(tokens[0], "WR") == 0) {
        ParseWR(tokens, count);
    }
    else {
        Parse(tokens, count);
    }
}
