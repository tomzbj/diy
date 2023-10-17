#ifndef _DRAW_H
#define _DRAW_H

enum {
    ZHFONT_11X11, ZHFONT_12X12, ZHFONT_14X14, ZHFONT_16X16
};

class ZHZ {
public:
    ZHZ(void (*pset_f)(signed short x, signed short y, unsigned long c))
    {
        _pset_f = pset_f;
    }
    void set_en_char_width(int en_char_width)
    {
        _en_char_width = en_char_width;
    }
    void setfc(unsigned long fc)
    {
        _fc = fc;
    }
    void setbc(unsigned long bc)
    {
        _bc = bc;
    }
    void setfont(int font)
    {
        _font = font;
        switch(_font) {
            case ZHFONT_11X11:
                _fontinfo.char_width = _fontinfo.char_height = 11;
                break;
            case ZHFONT_12X12:
                _fontinfo.char_width = _fontinfo.char_height = 12;
                break;
            case ZHFONT_14X14:
                _fontinfo.char_width = _fontinfo.char_height = 14;
                break;
            case ZHFONT_16X16:
                _fontinfo.char_width = _fontinfo.char_height = 16;
                break;
        }
    }
    void puts(int x, int y, const char* s);
    void putc(unsigned long icode, int x, int y);

private:
    struct {
        int char_width;
        int char_height;
//        const char* fname;
//        int mask_ofs;
    } _fontinfo;
    int _font;
    unsigned long _fc, _bc;
    int _en_char_width;

    void (*_pset_f)(signed short x, signed short y, unsigned long c);
};
#endif

