#ifndef _STRINGS_H
#define _STRINGS_H

enum {
    STRING_LANG_EN, STRING_LANG_MAX = STRING_LANG_EN
};

enum {
    STR_FREQ_1, STR_FREQ_2, STR_FREQ_3, STR_FREQ_4, STR_UNIT
};

unsigned char* STRING_Get(int strid, int lang);

#endif
