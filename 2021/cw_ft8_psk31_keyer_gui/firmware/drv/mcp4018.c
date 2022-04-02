#include "mcp4018.h"

#define  CMD_WRITE (ps->addr << 1)
#define  CMD_READ (CMD_WRITE + 1)

void MCP4018_Write(zi2c_t* ps, unsigned char data)
{
    ps->start_f(ps);
    ps->writebyte_f(ps, CMD_WRITE);
    ps->writebyte_f(ps, data);
    ps->stop_f(ps);
}
