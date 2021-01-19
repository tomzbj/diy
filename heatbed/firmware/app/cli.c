#include <stdlib.h>
#include "platform.h"
#include "misc.h"

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
            bool lsb = crc & 0x1;
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
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "zt_ntasks") == 0) {
        xprintf("%d\n", zt_get_num_tasks());
    }
    else if(strcasecmp(tokens[1], "t") == 0) {
        xprintf("%.1f\n", pdc->data.t_pv);
    }
    else if(strcasecmp(tokens[1], "all1") == 0) {
        static int count = 0;
        if(count % 10 == 0) {
            xprintf("|%6s|%6s|%6s|%6s|%6s|%6s|%6s|\n", "T_PV", "T_SP", "PID_P",
                "PID_I", "PID_D", "OUTPUT", "T_STEP");
        }
        xprintf("|%6.1f|%6.1f|%6.1f|%6.1f|%6.1f|%6d|%6.1f|\n", pdc->data.t_pv,
            pdc->status.t_sp, pdc->cfg.p, pdc->cfg.i, pdc->cfg.d,
            pdc->status.pwm, pdc->status.t_step);
//        xprintf("|%6d|%6d|%6d|%6d|%6d|%6d|\n", (int)pdc->data.t_pv, (int)pdc->status.t_sp, (int)pdc->cfg.p, (int)pdc->cfg.i, (int)pdc->cfg.d, (int)pdc->status.pwm);
        count++;
    }
}

static void ParseWR(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
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
    else if(strcasecmp(tokens[1], "pwm") == 0) {
        int pwm = strtol(tokens[2], NULL, 10);
        TIM_SetCompare1(TIM15, pwm);
        ok();
    }
    else if(strcasecmp(tokens[1], "sp") == 0) {
        int sp = strtof(tokens[2], NULL);
        pdc->status.t_sp = sp;
//        pid_clear_sum();
        DC_Update();
        ok();
    }
    else if(strcasecmp(tokens[1], "pid") == 0) {
        pdc->cfg.p = strtof(tokens[2], NULL);
        pdc->cfg.i = strtof(tokens[3], NULL);
        pdc->cfg.d = strtof(tokens[4], NULL);
        DC_Update();
        ok();
    }
    else if(strcasecmp(tokens[1], "debug") == 0) {
        pdc->status.debug = !(pdc->status.debug);
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

void CLI_Parse(const unsigned char* msg, int size) //, int type)
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
        token = strtok(NULL, seps); // Get next token:
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
