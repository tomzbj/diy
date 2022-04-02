#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "platform.h"

#define MSG_LEN 128

static void ParseRD(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "cfg") == 0 && count == 2) {
        printf("%d %d %d\n", pdc->cfg.p, pdc->cfg.i, pdc->cfg.d);
    }
    else if(strcasecmp(tokens[1], "adc") == 0 && count == 2) {
        for(int i = 0; i < 6; i++) {
            printf("%6lu", pdc->data.ch[i]);
        }
        printf("\n");
    }
    else if(strcasecmp(tokens[1], "ch") == 0 && count == 2) {
        int min_ch[6], max_ch[6];
        int max = 0, min = 2147483647;
        for(int i = 0; i < 6; i++) {
            max_ch[i] = pdc->data.ch[i];
            min_ch[i] = pdc->data.ch[i];
            if(max < max_ch[i])
                max = max_ch[i];
            if(min > min_ch[i])
                min = min_ch[i];
        }
        for(int i = 0; i < 6; i++) {
            max_ch[i] = max - max_ch[i];
            min_ch[i] = min_ch[i] - min;
        }
        for(int i = 0; i < 6; i++) {
            printf("%6d", max_ch[i]);
        }
        for(int i = 0; i < 6; i++) {
            printf("%6d", min_ch[i]);
        }
        printf("\n");
    }
}

static void ParseWR(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "STOP") == 0 && count == 2) {
        zt_stop(1);
        MOTOR_Set(MOTOR_L, 0);
        MOTOR_Set(MOTOR_R, 0);
        ok();
    }
    else if(strcasecmp(tokens[1], "PID") == 0 && count == 5) {
        int p = strtod(tokens[2], NULL);
        int i = strtod(tokens[3], NULL);
        int d = strtod(tokens[4], NULL);
        pdc->cfg.p = p;
        pdc->cfg.i = i;
        pdc->cfg.d = d;
        DC_Update();
        ok();
    }
    else if(strcasecmp(tokens[1], "ZT_START") == 0 && count == 3) {
        int n = strtod(tokens[2], NULL);
        zt_start(n);
    }
    else if(strcasecmp(tokens[1], "ZT_STOP") == 0 && count == 3) {
        int n = strtod(tokens[2], NULL);
        zt_stop(n);
    }
    else if(strcasecmp(tokens[1], "ML") == 0 && count == 3) {
        int speed = strtol(tokens[2], NULL, 10);
        MOTOR_Set(MOTOR_L, speed);
        ok();
    }
    else if(strcasecmp(tokens[1], "MR") == 0 && count == 3) {
        int speed = strtol(tokens[2], NULL, 10);
        MOTOR_Set(MOTOR_R, speed);
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

void CLI_Parse(const char* raw_frame, int size)
{
    char* tokens[8], * token;
    char seps[] = "?# ,", string[MSG_LEN];
    int i, len, count = 0;

    bzero(string, MSG_LEN);
    strncpy(string, raw_frame, size);
    for(i = 0; i < size; i++) {
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
