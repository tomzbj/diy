#include <ctype.h>
#include "misc.h"
#include "platform.h"

#define MSG_LEN 256

#define ok() printf("ok\n");

static void ParseRD(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "sp") == 0) {
        register unsigned char* sp asm("sp");
        printf("%08lX\n", (unsigned long)sp);
    }
    else if(strcasecmp(tokens[1], "freq") == 0) {
        printf("%d %lu\n", HXTAL_VALUE, rcu_clock_freq_get(CK_SYS));
        ok();
    }
    else if(strcasecmp(tokens[1], "all1") == 0) {
        printf("%d %d %d %d\n", pdc->state.ampl, pdc->state.waveform,
            pdc->state.curr_preset, pdc->cfg.unit);
    }
    else if(strcasecmp(tokens[1], "tab") == 0) {
        DDS_ViewTab();
    }
    else if(strcasecmp(tokens[1], "dma_cycles") == 0) {
        int t1 = DWT->CYCCNT;
        DDS_DMA_FTF();
        int t2 = DWT->CYCCNT;
        printf("%d\n", t2 - t1);
    }
}

static void ParseWR(char* const tokens[], int count)
{
//    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "DDS_START") == 0) {
        DDS_Start();
        ok();
    }
    else if(strcasecmp(tokens[1], "DDS_STOP") == 0) {
        DDS_Stop();
        ok();
    }
    else if(strcasecmp(tokens[1], "DDS_FREQ") == 0) {
        DDS_SetFreq(atol(tokens[2]));
        ok();
    }
    else if(strcasecmp(tokens[1], "ZT_STOP") == 0) {
        int tid = atoi(tokens[2]);
        zt_stop(tid);
        ok();
    }
    else if(strcasecmp(tokens[1], "ZT_START") == 0) {
        int tid = atoi(tokens[2]);
        zt_start(tid);
        ok();
    }
}

static void Parse(char* const tokens[], int count)
{
    if(strcasecmp(tokens[0], "TEST") == 0)
        printf("TEST OK\n");
    else if(strcasecmp(tokens[0], "REBOOT") == 0) {
        printf("System Reboot.\n\n");
        NVIC_SystemReset();
    }
}

void CLI_Parse(const void* pmsg, int size, int source)
{
    char* tokens[8], * token;
    char seps[] = "? ,#\r\n", string[MSG_LEN];
    int i, len, count = 0;
    char* msg = (char*)pmsg;

    bzero(string, MSG_LEN);
    strncpy(string, msg, size);
    for(i = 0; i < size; i++) {
        if(isalpha((int)string[i]))
            string[i] = toupper((int)string[i]);
    }

    len = strlen(string);
    while(string[len - 1] == '\n' || string[len - 1] == '\r') {
        string[len - 1] = '\0';
        len--;
    }

    token = strtok(string, seps);
    while(token != NULL) {

        tokens[count] = token;
        count++;
        token = strtok(NULL, seps);    // Get next token:
    }
    if(count == 0)
        return;

    if(strcasecmp(tokens[0], "RD") == 0)
        ParseRD(tokens, count);
    else if(strcasecmp(tokens[0], "WR") == 0)
        ParseWR(tokens, count);
    else
        Parse(tokens, count);
}
