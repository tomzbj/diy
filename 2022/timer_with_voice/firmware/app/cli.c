#include "misc.h"
#include "platform.h"

#define MSG_LEN 128

#define ok() printf("ok.\n")

static void ParseRD(char* const tokens[], int count)
{
//    DataConfig_t* p = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "zt_ntasks") == 0) {
        printf("%d\n", zt_get_num_tasks());
    }
//    else if(strcasecmp(tokens[1], "findfile") == 0) { FLASH_FindFile(tokens[2]); }
}

static void ParseWR(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "play") == 0) {
        AUDIO_StartPlay(tokens[2]);
    }
    else if(strcasecmp(tokens[1], "baud") == 0) {
        unsigned long baud = strtoul(tokens[2], NULL, 10);
        USART_SetBaudrate(baud);
        printf("%d\n", USART2->BRR);
    }
    else if(strcasecmp(tokens[1], "set_defaults") == 0) {
        memset(&(pdc->cfg), 0, sizeof(pdc->cfg));
        DC_Update();
        ok();
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
}

static void Parse(char* const tokens[], int count)
{
    if(strcasecmp(tokens[0], "TEST") == 0) {
        printf("TEST OK\n");
    }
    else if(strcasecmp(tokens[0], "REBOOT") == 0) {
        printf("Done.\n");
        NVIC_SystemReset();
    }
}

void CLI_Parse(const void* pmsg, int size, int source)
{
    unsigned char* msg = (unsigned char*)pmsg;
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
