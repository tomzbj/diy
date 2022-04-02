#ifndef _STRINGS_H
#define _STRINGS_H

#define STR_BAUDRATE 0
#define STR_ADDRESS 1
#define STR_CUR_STEP 2
#define STR_DISP_BRIGHTNESS 3
#define STR_LANGUAGE 4
#define STR_PRESET 5
#define STR_MEAS 6
#define STR_VIEW_SYSCFG 7
#define STR_LANG_CN 8
#define STR_LANG_EN 9

unsigned char* STRING_Get(int strid);

#endif
