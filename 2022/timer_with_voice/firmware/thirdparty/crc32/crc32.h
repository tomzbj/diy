#ifndef _CRC32_H
#define _CRC32_H

#define crc32 crc32_lut

unsigned long crc32_lut(unsigned long crc, const void* msg, int size);
unsigned long crc32_soft(unsigned long crc, const void* msg, int size);

#endif
