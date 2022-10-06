#include "zaprs.h"
//#include "bitmanip.h"
#include <string.h>
#include <stdio.h>

static struct {
    zaprs_cb_t cb;
    volatile unsigned short crc;
    volatile unsigned short currbyte;
    unsigned char callsign[8];
    unsigned char tx_buf[4096];
    int tx_pos;
    int tx_nbits;
    int flag_tx;
} g;

static void push_bit(int bit)
{
    if(g.tx_nbits < sizeof(g.tx_buf)) {
        g.tx_buf[g.tx_nbits] = bit;
        g.tx_nbits++;
    }
}

//发送同步字0b01111110
static void transmit_sync(void)
{
    push_bit(0);
    push_bit(1);
    push_bit(1);
    push_bit(1);
    push_bit(1);
    push_bit(1);
    push_bit(1);
    push_bit(0);
}

//发送一位. 发送时若有五个连续的1, 则自动在第五个1后面插入一个0
static void stuff_transmit(unsigned char bit)
{
    g.currbyte = (g.currbyte << 1) | bit;
    push_bit(bit);
    if((g.currbyte & 0x1f) == 0x1f) {    //连续发送了五个1之后插入一个0
        g.currbyte = (g.currbyte << 1);
        push_bit(0);
    }
}

//按位更新CRC
static void crc_update_bit(unsigned char crc_bit)
{
    unsigned char shiftbit;

    shiftbit = 0x0001 & (unsigned char)g.crc;
    g.crc >>= 1;
    if(shiftbit != crc_bit) {
        g.crc ^= 0x8408;
    }
}

static void crc_update_byte(unsigned char byte)
{
    unsigned char i;
    for(i = 0; i < 8; i++) {
        crc_update_bit((byte & (1 << i)) ? 1 : 0);
    }
}

static void transmit_byte(unsigned char byte)
{
    g.cb.tx_char_f(byte);
    unsigned char i;
    for(i = 0; i < 8; i++) {
        stuff_transmit((byte & (1 << i)) ? 1 : 0);
    }
    crc_update_byte(byte);
}

static void dataframe_start(void)
{
    g.crc = 0xffff;
    g.currbyte = 0;
}

void zaprs_init(zaprs_cb_t* cb, const unsigned char* callsign)
{
    g.cb = *cb;
    memcpy(g.callsign, callsign, sizeof(g.callsign));
}

void zaprs_tx(const unsigned char* msg)
{
    unsigned char i, crcl, crch;
    const unsigned char* addr_dest = (const unsigned char*)"APRS  ";
    const unsigned char* addr_digipeater = (const unsigned char*)"WIDE1 ";

    g.tx_nbits = 0;

    for(i = 0; i < 5; i++)
        transmit_sync();
    dataframe_start();

    for(const unsigned char* p = addr_dest; *p; p++)
        transmit_byte((*p) << 1);
    transmit_byte(0xe0);

    for(const unsigned char* p = g.callsign; *p; p++)
        transmit_byte((*p) << 1);
    transmit_byte(0xe2);

    for(const unsigned char* p = addr_digipeater; *p; p++)
        transmit_byte((*p) << 1);

    transmit_byte(0x63);
    transmit_byte(0x03);
    transmit_byte(0xf0);
    for(const unsigned char* p = msg; *p; p++)
        transmit_byte(*p);
    g.crc ^= 0xffff;
    crcl = g.crc & 0xff;
    crch = (g.crc >> 8) & 0xff;
    transmit_byte(crcl);
    transmit_byte(crch);
//    transmit_byte(0x38);
//    transmit_byte(0x76);

    for(i = 0; i < 5; i++)
        transmit_sync();
    g.tx_pos = 0;
    g.cb.tx_start_f();
    g.flag_tx = 1;

    printf("%d bits\n", g.tx_nbits);
}

void zaprs_poll(void)
{
    if(g.flag_tx) {
        g.cb.tx_symbol_f(g.tx_buf[g.tx_pos]);
        g.tx_pos++;
        if(g.tx_pos > g.tx_nbits) {
            g.flag_tx = 0;
            g.cb.tx_end_f();
        }
    }
}
