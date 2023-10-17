#ifndef _ZGATE_H
#define _ZGATE_H

#include "zpin.h"

enum {
    GATE_TYPE_AND = 0x8,            // 1000
    GATE_TYPE_OR = 0xe,             // 1110
    GATE_TYPE_NAND = 0x7,           // 0111
    GATE_TYPE_NOR = 0x1,            // 0001
    GATE_TYPE_XOR = 0x6,            // 0110
    GATE_TYPE_NOT = GATE_TYPE_NAND  // 2 inputs are same pin
};

class ZGate {
public:
    ZGate(zpin_t in1, zpin_t in2, zpin_t out, int type)    //
    : _in1(in1), _in2(in2), _out(out), _type(type)
    {
    }
    void update(void)
    {
        int in = 0;
        in |= (zpin_read(_in1)) ? 1 : 0;
        in |= (zpin_read(_in2)) ? 2 : 0;
        out_state = (_type >> in) & 1;
    }
    void output(void)
    {
        zpin_write(_out, out_state);
    }

private:
    zpin_t _in1, _in2, _out;
    int _type;
    bool out_state;
};

#endif
