#include <ctype.h>
#include "misc.h"
#include "platform.h"

#define MSG_LEN 256
#define ok() printf("ok\n");

static void ParseRD(char* const tokens[], int count)
{
    if(0) {
    }
}

static void ParseWR(char* const tokens[], int count)
{
    if(0) {
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

void CLI_Parse(const void* pmsg, int size)
{
    const char* msg = (const char*)pmsg;
    char* tokens[8], * token;
    char seps[] = "? ,#\r\n", string[MSG_LEN];
    int i, len, count = 0;

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

    if(strcasecmp(tokens[0], "RD") == 0) {
        ParseRD(tokens, count);
    }
    else if(strcasecmp(tokens[0], "WR") == 0) {
        ParseWR(tokens, count);
    }
    else
        Parse(tokens, count);
}
