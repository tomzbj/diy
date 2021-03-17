#include "misc.h"
#include "platform.h"

#define MSG_LEN 128

#define ok() xprintf("ok.\n")

static void ParseRD(char* const tokens[], int count)
{
//    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "zt_ntasks") == 0) {
        xprintf("%d\n", zt_get_num_tasks());
    }
}

static void ParseWR(char* const tokens[], int count)
{
//    DataConfig_t* pdc = DC_Get();

    if(0) {
    }
    else if(strcasecmp(tokens[1], "zt_start") == 0) {
        int tid = atol(tokens[2]);
        zt_start(tid);
        ok();
    }
    else if(strcasecmp(tokens[1], "zt_stop") == 0) {
        int tid = atol(tokens[2]);
        zt_stop(tid);
        ok();
    }
    else if(strcasecmp(tokens[1], "dac") == 0) {
        int val = atol(tokens[2]);
        DAC_SetChannel1Data(DAC_Align_12b_R, val);
        ok();
    }
}

static void Parse(char* const tokens[], int count)
{
    if(strcasecmp(tokens[0], "TEST") == 0) {
        xprintf("TEST OK\n");
    }
    if(strcasecmp(tokens[0], "TEST2") == 0) {
        zu_writedata("TEST2 OK\n", 9, 0);
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
