#include "zhz.h"
#include "ugui.h"
#include <string.h>

/*
 zhfont_t ZHFONT_11X11 = {11, 11, "HZK11US", 0};
 zhfont_t ZHFONT_12X12 = {12, 12, "HZK12US", 0};
 zhfont_t ZHFONT_14X14 = {14, 14, "HZK14US", 0};
 zhfont_t ZHFONT_16X16 = {16, 16, "HZK16US", 0};
 */

/*
 static struct {
 void (*pset_f)(unsigned short x, unsigned short y, unsigned long c);
 zhfont_t* font;
 unsigned long fc, bc;
 int en_char_width;
 } g;
 */

#define UNI2GB_TAB_SIZE (29780 / 4)
#define UNI2GB_TAB_OFS 0

extern const unsigned short uni2gb_tab[];

extern const int unicode_tab_size;
extern const unsigned short unicode_tab[];

void ZHZ::puts(int x, int y, const char* s)
{
    unsigned short unicode = 0;
    while(*s) {
        if((*s & 0xe0) == 0xe0) {    // utf8 3bytes
            unicode = ((unsigned short)*s << 12)
                | ((unsigned short)(*(s + 1) & 0x3f) << 6) | (*(s + 2) & 0x3f);
            s += 3;
            putc(unicode, x, y);
            x += _fontinfo.char_width;
        }
        else if((*s & 0xc0) == 0xc0) {      // utf8 2bytes
            unicode = ((unsigned short)*s << 6) | (*(s + 1) & 0x3f);
            s += 2;
            putc(unicode, x, y);
            x += _fontinfo.char_width;
        }
        else if(*s <= 127) {
            UG_PutChar(*s, x, y, _fc, _bc);
            s++;
            x += _en_char_width;
        }
        else
            continue;
    }
//    printf("\n");
}

extern unsigned char hzk11us_font_data[];
extern unsigned char hzk12us_font_data[];
extern unsigned char hzk14us_font_data[];
extern unsigned char hzk16us_font_data[];

static void read_font_data(unsigned short icode, int height, unsigned char* buf)
{
    int pos = 0;

    extern unsigned short uni_smlib_tab[];
    extern int uni_smlib_tab_count;

    for(int i = 0; i < uni_smlib_tab_count; i++) {
        if(uni_smlib_tab[i] == icode)
            pos = i;
    }

    switch(height) {
        case 11:
            memcpy(buf, hzk11us_font_data + pos * 22, 22);
            break;
        case 12:
            memcpy(buf, hzk12us_font_data + pos * 24, 24);
            break;
        case 14:
            memcpy(buf, hzk14us_font_data + pos * 28, 28);
            break;
        case 16:
            memcpy(buf, hzk16us_font_data + pos * 32, 32);
            break;
    }
}

void ZHZ::putc(unsigned long icode, int x, int y)
{
    unsigned short i, j, k, xo, yo, c, bn, actual_char_width;
    unsigned long b, index;
    unsigned char buf[128];

    yo = y;
    bn = _fontinfo.char_width;
    if(!bn)
        return;
    bn >>= 3;
    if(_fontinfo.char_width % 8)
        bn++;
    actual_char_width = _fontinfo.char_width;

    read_font_data(icode, _fontinfo.char_height, buf);

    index = 0;

    for(j = 0; j < _fontinfo.char_height; j++) {
        xo = x;
        c = actual_char_width;
        for(i = 0; i < bn; i++) {
            b = buf[index++];
            for(k = 0; (k < 8) && c; k++) {
                (b & 0x80) ? (_pset_f(xo, yo, _fc)) : (_pset_f(xo, yo, _bc));
                b <<= 1;
                xo++;
                c--;
            }
        }
        yo++;
    }
}
