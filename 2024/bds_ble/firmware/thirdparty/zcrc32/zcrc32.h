/* Author: tomzbj  Updated: 2024-04-28 Rev: 01*/

#ifndef _ZCRC32_H
#define _ZCRC32_H

class ZCRC32 {
public:
    static uint32_t calc(const void* msg, int size)
    {
        return calc(0xffffffff, msg, size);
    }

    static uint32_t calc(uint32_t crc, const void* pmsg, int size)
    {
        const uint32_t polynormial = 0xedb88320;
        auto msg = (uint8_t*)pmsg;
        for(int i = 0; i < size; i++) {
            crc ^= msg[i];
            for(int j = 0; j < 8; j++) {
                int lsb = crc % 2;
                crc >>= 1;
                if(lsb)
                    crc ^= polynormial;
            }
        }
        return crc;
    }

private:
};

#endif
