/* Author: tomzbj  Updated: 2024-04-16 Rev3 */

#ifndef _ZCLI_H
#define _ZCLI_H

#include <cctype>

class ZCli {
public:

    typedef void (*cmd_func_t)(const char* tokens[]);
    typedef struct {
        cmd_func_t func;
        const char* type;
        const char* name;
    } zcli_cmd_t;

    ZCli(int max_cmds)
    {
        _cmd = new zcli_cmd_t[max_cmds];
        _num_cmds = 0;
        _max_cmds = max_cmds;
    }
    int bind(const char* name, cmd_func_t cmd_f)
    {
        return bind(name, "", cmd_f);
    }
    int bind(const char* name, const char* cmd_type, cmd_func_t cmd_f)
    {
        if(_num_cmds >= _max_cmds)
            return -1;
        _cmd[_num_cmds].func = cmd_f;
        _cmd[_num_cmds].type = cmd_type;
        _cmd[_num_cmds].name = name;
        _num_cmds++;
        return 0;
    }
    void parse(const void* pmsg, int size)
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

private:
    zcli_cmd_t* _cmd;
    int _num_cmds, _max_cmds;
    const int MSG_LEN = 256;
};

#endif
