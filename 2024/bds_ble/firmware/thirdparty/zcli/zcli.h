/* Author: tomzbj  Updated: 2024-04-13 */

#ifndef _ZCLI_H
#define _ZCLI_H

typedef void (*cmd_func_t)(const char* tokens[]);
typedef struct {
    cmd_func_t func;
    const char* type;
    const char* name;
} zcli_cmd_t;

class ZCli {
public:
    ZCli(int max_cmds)
    {
        _cmd = new zcli_cmd_t[max_cmds];
        _num_cmds = 0;
        _max_cmds = max_cmds;
    }
    void parse(const void* msg, int size);
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
private:
    zcli_cmd_t* _cmd;
    int _num_cmds, _max_cmds;
};

#endif
