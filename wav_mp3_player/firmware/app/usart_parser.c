#include "platform.h"
#include <string.h>

void USART_Parser(const unsigned char* msg, int size)
{
    if(0) {
    }
    else if(memcmp(msg, "##", 2) == 0 || msg[0] == '?') {
        CLI_Parse(msg, size);
    }
}
