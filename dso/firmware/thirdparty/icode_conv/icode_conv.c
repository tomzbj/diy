#include "misc.h"
#include "spiflash.h"
#include "icode_conv.h"

unsigned short unicode_gb2312(unsigned short in_chr)
{
    int min, max, mid;

    min = 0;
    max = UNI2GB_TAB_SIZE - 1;
    while(min <= max) {
        unsigned short uni, gb;
        unsigned long rec;

        mid = min + (max - min) / 2;

        uni2gb_rec_get(UNI2GB_TAB_OFS + mid * 4, 4, &rec);

        gb = rec & 0xffff;
        uni = rec >> 16;

        if(in_chr < uni)
            max = mid - 1;
        else if(in_chr > uni)
            min = mid + 1;
        else
            return gb;
    }
    return 0xffff;
}

void str_utf2gb(const unsigned char* str, unsigned char* str_out)
{
    unsigned short unicode, gb2312;

    while(*str) {
        if((*str & 0xe0) == 0xe0) {   // utf8 3bytes
            unicode = ((unsigned short)*str << 12)
                | ((unsigned short)(*(str + 1) & 0x3f) << 6)
                | (*(str + 2) & 0x3f);
            str += 3;
        }
        else if((*str & 0xc0) == 0xc0) {      // utf8 2bytes
            unicode = ((unsigned short)*str << 6) | (*(str + 1) & 0x3f);
            str += 2;
        }
        else if(*str <= 127) {
            *str_out = *str;
            str++;
            str_out++;
            continue;
        }

        else
            continue;
        gb2312 = unicode_gb2312(unicode);
        *str_out = gb2312 >> 8;
        str_out++;
        *str_out = gb2312 & 0xff;
        str_out++;
    }
}

