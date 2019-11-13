/// @file menu.c
/// @brief 菜单显示程序.
/// @author Zhang Hao, stavrosatic@gmail.com
/// @version R0.1
/// @date 2017-10-12
/// @note 菜单项：
/// @note 1. 波特率设置: 2400, 4800, 9600, 19200, 38400, 115200.
/// @note 2. 设备地址设置: 1~32 (动态添加).
/// @note 3. 当前分度表选择: 从已有分度表列表中选择(动态添加).
/// @note 4. 4~20mA对应温度范围设置.

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "conf.h"
#include "disp.h"
//#include "index.h"
//#include "readmask.h"
#include "ugui.h"
#include "strings.h"
#include "current.h"

#define MAX_NAME_LEN 24
#define MAX_MENU_ITEMS 64
#define ITEMS_PER_PAGE 3
#define TOPLEVEL 254

typedef struct {
    unsigned char parent;   // 上层菜单项, TOPLEVEL则为顶层菜单     可减为1字节
    bool isdir;             // 是否是目录, 是则进入下层菜单, 否则执行操作   1字节
    unsigned short val;      // 菜单项值     可减为1字节 (2400的倍数)
    char *name;             // 菜单项名称       可减为1字节 (字符串序号)
} MenuItem_t;

typedef enum {
    MENU_ACTION_UP, MENU_ACTION_DOWN, MENU_ACTION_BACK, MENU_ACTION_RUN
} MenuAction_t;

const unsigned long baud_rates[] = {2400, 4800, 9600, 19200, 38400, 115200};

static struct {
    int item_count;
    int menu_group;
    int menu_pos;
//    char current_dir[MAX_NAME_LEN + 2];
    MenuItem_t menu[MAX_MENU_ITEMS];
} g;

static struct {
    int baudrate;
    int device_addr;
    int current_steps;
    int disp_brightness;
    int language;
} dirs = {999, 999, 999, 999, 999};

/// @brief 添加一个菜单项.
/// @param parent 上级菜单项.
/// @param isdir 是否是目录项.
/// @param name[] 菜单项名称.
/// @param val 菜单项值.
/// @retval 如果分配内存失败, 返回-1, 否则返回0.
static int AddItem(int parent, bool isdir, unsigned char* name,
    unsigned long val)
{
    int ret, namelen = 0;
    g.menu[g.item_count].parent = parent;
    g.menu[g.item_count].isdir = isdir;

    namelen = strlen((char*)name);
    if(namelen > MAX_NAME_LEN)
        namelen = MAX_NAME_LEN;
    g.menu[g.item_count].name = malloc(namelen + 1);
    bzero(g.menu[g.item_count].name, namelen + 2);
    if(g.menu[g.item_count].name == NULL)
        return -1;
    strncpy(g.menu[g.item_count].name, (char*)name, namelen);
    g.menu[g.item_count].val = val;
    ret = g.item_count;
    g.item_count++;

    return ret;
}

/// @brief 根据名称查找菜单项.
/// @param name[] 待查找菜单项名称.
/// @retval 如果找到, 返回菜单项id, 未找到则返回-1.
static int GetItemIdByName(unsigned char* name)
{
    int i;
    for(i = 0; i < sizeof(g.menu) / sizeof(g.menu[0]); i++) {
        if(strcmp((char*)name, g.menu[i].name) == 0)
            return i;
    }
    return -1;
}

/// @brief 计算同组菜单项总数.
/// @param group 菜单项组id.
/// @retval 同组菜单项总数.
static int GetItemCountByGroup(int group)
{
    int i, count = 0;
    for(i = 0; i < sizeof(g.menu) / sizeof(g.menu[0]); i++) {
        if(g.menu[i].parent == group && strlen(g.menu[i].name))
            count++;
    }
    return count;
}

/// @brief 根据光标位置查找菜单项.
/// @param group 菜单项组id.
/// @param pos 光标位置.
/// @retval  如果找到, 返回菜单项id, 否则返回-1.
static int GetItemIndexByGroupPos(int group, int pos)
{
    int i, ipos = 0;
    for(i = 0; i < sizeof(g.menu) / sizeof(g.menu[0]); i++) {
        if(g.menu[i].parent == group) {
            if(pos == ipos)
                return i;
            ipos++;
        }
    }
    return -1;
}

/// @brief 菜单初始化.
/// @param None
/// @retval None
void MENU_Config(void)
{
    int i, parentid;
//    unsigned char buf[12];

    for(i = 0; i < g.item_count; i++) {
        free(g.menu[i].name);
    }
    g.item_count = 0;
    g.menu_group = TOPLEVEL;
    g.menu_pos = 0;

//    dirs.baudrate = AddItem(TOPLEVEL, true, STRING_Get(STR_BAUDRATE), 0);
//    dirs.device_addr = AddItem(TOPLEVEL, true, STRING_Get(STR_ADDRESS), 0);
    dirs.current_steps = AddItem(TOPLEVEL, true, STRING_Get(STR_CUR_STEP), 0);
    dirs.disp_brightness = AddItem(TOPLEVEL, true,
        STRING_Get(STR_DISP_BRIGHTNESS), 0);
//    dirs.language = AddItem(TOPLEVEL, true, STRING_Get(STR_LANGUAGE), 0);

    /*
     parentid = GetItemIdByName(STRING_Get(STR_BAUDRATE));
     for(i = 0; i < sizeof(baud_rates) / sizeof(baud_rates[0]); i++) {
     bzero(buf, sizeof(buf));
     sprintf((char*)buf, "%lu", baud_rates[i]);
     AddItem(parentid, false, buf, baud_rates[i]);
     }

     parentid = GetItemIdByName(STRING_Get(STR_ADDRESS));
     for(i = 1; i <= 32; i++) {
     bzero(buf, sizeof(buf));
     sprintf((char*)buf, "%d", i);
     AddItem(parentid, false, buf, i);
     }
     */
    parentid = GetItemIdByName(STRING_Get(STR_CUR_STEP));
    AddItem(parentid, false, (unsigned char*)"1/2/5", CURRENT_PREF_S125);
    AddItem(parentid, false, (unsigned char*)"1/1.5/2...", CURRENT_PREF_S123);
    AddItem(parentid, false, (unsigned char*)"R5 Series", CURRENT_PREF_R5);
    AddItem(parentid, false, (unsigned char*)"R10 Series", CURRENT_PREF_R10);

    parentid = GetItemIdByName(STRING_Get(STR_DISP_BRIGHTNESS));
    AddItem(parentid, false, (unsigned char*)"0", 0);
    AddItem(parentid, false, (unsigned char*)"1", 1);
    AddItem(parentid, false, (unsigned char*)"2", 2);
    AddItem(parentid, false, (unsigned char*)"3", 3);
    AddItem(parentid, false, (unsigned char*)"4", 4);
    AddItem(parentid, false, (unsigned char*)"5", 5);

    parentid = GetItemIdByName(STRING_Get(STR_LANGUAGE));
    AddItem(parentid, false, STRING_Get(STR_LANG_CN), STR_LANG_CN);
    AddItem(parentid, false, STRING_Get(STR_LANG_EN), STR_LANG_EN);

//    parentid = GetItemIdByName(STRING_Get(STR_VIEW_SYSCFG));
}

/// @brief 检查菜单项是否选中. 
/// @param index 待检查菜单项id.
/// @retval true/false: 选中/未选中.
static bool ItemChecked(int index)
{
    char buf[12];
    GlobalConfig_t* conf = GlobalConfigRead();
    bzero(buf, sizeof(buf));

    if(g.menu[index].isdir)
        return false;

    if(g.menu[index].parent == dirs.baudrate) {
        if(conf->baudrate == g.menu[index].val)
            return true;
    }
    else if(g.menu[index].parent == dirs.device_addr) {
        if(conf->device_addr == g.menu[index].val)
            return true;
    }
    else if(g.menu[index].parent == dirs.current_steps) {
        if(conf->current_steps == g.menu[index].val)
            return true;
    }
    else if(g.menu[index].parent == dirs.disp_brightness) {
        if(conf->disp_brightness == g.menu[index].val)
            return true;
    }
    else if(g.menu[index].parent == dirs.language) {
        if(conf->language == g.menu[index].val)
            return true;
    }
    return false;
}

/// @brief 更新配置
/// @param index 待更新菜单项id
/// @retval None
static void UpdateConfig(int index)
{
    GlobalConfig_t *conf = GlobalConfigRead();

    if(g.menu[index].parent == dirs.baudrate) {
        conf->baudrate = g.menu[index].val;
    }
    else if(g.menu[index].parent == dirs.device_addr) {
        conf->device_addr = g.menu[index].val;
    }
    else if(g.menu[index].parent == dirs.current_steps) {
        conf->current_steps = g.menu[index].val;
    }
    else if(g.menu[index].parent == dirs.disp_brightness) {
        conf->disp_brightness = g.menu[index].val;
    }
    else if(g.menu[index].parent == dirs.language) {
        conf->language = g.menu[index].val;
    }
    GlobalConfigUpdate(conf);
}

/// @brief 显示菜单项. 
/// @param group 当前菜单项组id.
/// @param pos 光标位置.
/// @retval None
static void View(int group, int pos)
{
    static int old_group = -1, old_pos = -1;
    int i, ipos = 0, ypos = 0;
    unsigned char buf[24];
    if(group != old_group || pos != old_pos)
        DISP_Clear();
    UG_FontSelect(&FONT_8X12);
    old_group = group;
    old_pos = pos;
    for(i = 0; i < g.item_count; i++) {
        if(g.menu[i].parent == group && strlen(g.menu[i].name)) {
            if(pos < ITEMS_PER_PAGE) {      // cursor in this page 
                sprintf((char*)buf, " %s", g.menu[i].name);
                if(ItemChecked(i))
                    strcat((char*)buf, " (*)");
                else
                    strcat((char*)buf, "    ");
                if(pos == ypos) {             // show cursor
                    buf[0] = '>';
                }
                UG_PutString(0, ypos * 11 - 1, (char*)buf);
                ypos++;
                if(ypos >= ITEMS_PER_PAGE)  // all 4 items shown
                    break;
            }
            else {
                ipos++;
                if(ipos >= ITEMS_PER_PAGE) {  // next page
                    pos -= ITEMS_PER_PAGE;
                    ipos -= ITEMS_PER_PAGE;
                }
                if(pos < 0)                 // out of range
                    break;
            }
        }
    }
}

/// @brief 显示菜单.
/// @param None
/// @retval None
void MENU_View(void)
{
    View(g.menu_group, g.menu_pos);
}

/// @brief 检查是否在顶层菜单.
/// @param None
/// @retval None
int MENU_IsToplevel(void)
{
    if(g.menu_group == TOPLEVEL)
        return 1;
    else
        return 0;
}

/// @brief 在当前菜单项上执行操作. 
/// @param action 操作类型, UP/DOWN/BACK/RUN: 光标上移/下移, 退回上层菜单, 执行.
/// @retval None
void MENU_Action(MenuAction_t action)
{
    int index = GetItemIndexByGroupPos(g.menu_group, g.menu_pos);
    switch(action) {
        case MENU_ACTION_UP:
            if(g.menu_pos > 0)
                g.menu_pos--;
            break;
        case MENU_ACTION_DOWN:
            if(g.menu_pos < GetItemCountByGroup(g.menu_group) - 1)
                g.menu_pos++;
            break;
        case MENU_ACTION_BACK:
            if(g.menu_group != TOPLEVEL)
                g.menu_group = g.menu[g.menu_group].parent;
            g.menu_pos = 0;
            break;
        case MENU_ACTION_RUN:
            if(g.menu[index].isdir == true) {
                g.menu_group = index;
                g.menu_pos = 0;
//                bzero(g.current_dir, sizeof(g.current_dir));
//                if(g.menu[GetItemIndexByGroupPos(index, 0)].isdir == false) {
//                    strcpy(g.current_dir, g.menu[index].name);
//                }
            }
            else {
                UpdateConfig(index);
                DISP_Clear();
            }
            break;
    }
    index = GetItemIndexByGroupPos(g.menu_group, g.menu_pos);
}
