#include "misc.h"
#include "platform.h"

const struct {
    int strid;
    char* strs[STRING_LANG_MAX + 1];
} g_string[] = {
//
    {STR_FREQ_1, {"FREQUENCY_1"}},    //
    {STR_FREQ_2, {"FREQUENCY_2"}},    //
    {STR_FREQ_3, {"FREQUENCY_3"}},    //
    {STR_FREQ_4, {"FREQUENCY_4"}},    //
    {STR_UNIT,   {"FREQUENCY UNIT"}},    //
    };

unsigned char* STRING_Get(int strid, int lang)
{
    int n_strings = sizeof(g_string) / sizeof(g_string[0]);
    if(lang <= STRING_LANG_MAX) {
        for(int i = 0; i < n_strings; i++) {
            if(g_string[i].strid == strid)
                return (unsigned char*)g_string[i].strs[lang];
        }
        return (unsigned char*)"";
    }
    else
        return (unsigned char*)"";
}
