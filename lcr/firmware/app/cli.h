#ifndef _CLI_H
#define _CLI_H

enum {
    CLI_TYPE_BINARY, CLI_TYPE_ASCII
};

void CLI_Parse(const unsigned char* msg, int size, int type);

#endif
