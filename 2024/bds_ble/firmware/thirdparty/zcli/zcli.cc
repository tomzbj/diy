/* Author: tomzbj  Updated: 2024-04-13 */

#include <ctype.h>
#include "misc.h"
#include "platform.h"

#define MSG_LEN 256
#define ok() printf("ok\n");

void ZCli::parse(const void* pmsg, int size)
{
    const char* msg = (const char*)pmsg;
    const char* tokens[8], * token;
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


    for(int i = 0; i < _num_cmds; i++) {
        if(strcasecmp("", _cmd[i].type) == 0) {
            if(strcasecmp(tokens[0], _cmd[i].name) == 0) {
                _cmd[i].func(tokens);
            }
        }
        else if(strcasecmp(tokens[0], _cmd[i].type) == 0) {
            if(strcasecmp(tokens[1], _cmd[i].name) == 0) {
                _cmd[i].func(tokens);
            }
        }
    }
}
