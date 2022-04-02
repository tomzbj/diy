#include <string.h>
#include "strings.h"
#include "misc.h"
#include "conf.h"

const char* g_string[2][10] = {
    {
        "设置485波特率",
        "设置485设备地址",
        "设置电流步进系列",
        "设置屏幕亮度",
        "设置界面语言",
        "预设:",
        "实测:",
        "查看系统信息",
        "简体中文",
        "英文",
    },
    {
        "485 Baudrate",
        "485 Address",
        "CURRENT STEP",
        "LCD BRIGHTNESS",
        "Interface Language",
        "PRESET",
        "ACTUAL",
        "View System Config",
        "Simp. Chinese",
        "English"
    }
};

unsigned char* STRING_Get(int strid)
{
    GlobalConfig_t* conf = GlobalConfigRead();
    if(conf->language == STR_LANG_CN) {
        static unsigned char buf[32];
        bzero(buf, 32);
        return buf;
    }
    else
        return (unsigned char*)g_string[1][strid];
}
