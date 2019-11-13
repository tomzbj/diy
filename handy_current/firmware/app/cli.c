#include "cli.h"
#include "misc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "disp.h"

#define MSG_LEN 64

static void Parse(char* const tokens[], int count)
{
    if(0) {
    }
    else if(strcasecmp(tokens[0], "TEST") == 0 && count == 1) {
        printf("CLI Test OK!\n");
    }
    /*    else if(strcasecmp(tokens[0], "CLS") == 0 && count == 2) {
     unsigned char patt = strtol(tokens[1], NULL, 16);
     DISP_Clear(patt);
     printf("CLS OK!\n");
     }
    else if(strcasecmp(tokens[0], "BR") == 0 && count == 2) {
        unsigned char br = strtol(tokens[1], NULL, 10);
        DISP_Brightness(br);
        printf("Brightness Set!\n");
    }
    */
    else if(strcasecmp(tokens[0], "REBOOT") == 0 && count == 1) {
        printf("Reboot.\n");
        SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_Flash);
        NVIC_SystemReset();
    }
}

void CLI_Parse(const char* msg, int size)
{
    char *tokens[8], *token;
    char seps[] = "?# ,", string[MSG_LEN];
    int len, count = 0;

    bzero(string, MSG_LEN);
    strncpy(string, msg, size);

    len = strlen(string);
    while(string[len - 1] == '\n' || string[len - 1] == '\r') {
        string[len - 1] = '\0';
        len--;
    }

    token = strtok(string, seps);
    while(token != NULL) {
        tokens[count] = token;
        count++;
        token = strtok(NULL, seps); // Get next token:
    }

    if(count > 0)
        Parse(tokens, count);
}
