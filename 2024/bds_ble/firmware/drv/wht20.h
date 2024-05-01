#ifndef _WHT20_H
#define _WHT20_H

#include "zi2c.h"
// i2c addr: 0x38

class WHT20 {
public:
//    typedef struct { uint16_t RH, T; } result_t;

    uint16_t RH, T;

    WHT20(ZI2C& i2c) : _i2c(i2c)
    {
    }
    void init(void)
    {
        auto status = _i2c.read_reg(0x71);
        if((status & 0x08) != 0x08) {
            uint8_t data[] = {0x08, 0x00};
            _i2c.write_regs(0xbe, data, 2);
        }
        measure();
    }

    void poll(void)
    {
        uint8_t buf[7];
        _i2c.read_raw(0x38, buf, 7);
        //        for(int i = 0; i < 9; i++) { printf("%02X ", buf[i]); } printf("\n");
        measure();
//        result_t result;
        RH = (buf[1] * 256 + buf[2]) * 10000UL / 65536;    //  + buf[3] >> 4;
        T = ((buf[3] & 0xf) * 4096 + buf[4] * 16 + (buf[5] >> 4)) * 20000UL
            / 65536 - 5000;
//        return result;
    }
    /* result_t read(void)
     {
     //        auto status = _i2c.read_reg(0x38);
     //        printf("%02x\n", status);

     uint8_t buf[9];
     _i2c.read_raw(0x38, buf, 9);
     //        for(int i = 0; i < 9; i++) { printf("%02X ", buf[i]); } printf("\n");
     measure();
     result_t result;
     result.RH = (buf[1] * 256 + buf[2]) * 10000UL / 65536;    //  + buf[3] >> 4;
     result.T = ((buf[3] & 0xf) * 4096 + buf[4] * 16 + (buf[5] >> 4))
     * 20000UL / 65536 - 5000;
     return result;
     } */

    void test(void)
    {
        auto status = _i2c.read_reg(0x38);
        printf("%02x\n", status);
    }
private:
    void measure(void)
    {
        uint8_t data[] = {0x33, 0x00};
        _i2c.write_regs(0xac, data, 2);    // do measure
    }
    static uint8_t crc8_soft(uint8_t crc, const void* msg, int size)
    {
        const uint8_t polynormial = 0x31;
        uint8_t* p = (uint8_t*)msg;
        for(int i = 0; i < size; i++) {
            crc ^= *p++;
            for(int j = 0; j < 8; j++) {
                int lsb = crc & 0x80;
                crc <<= 1;
                if(lsb)
                    crc ^= polynormial;
            }
        }
        return crc;
    }
    ZI2C& _i2c;
};

#endif
