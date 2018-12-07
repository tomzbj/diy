#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "spiflash.h"
#include "cmd.h"
#include "ff.h"
#include "usart.h"
#include "vs1003.h"
#include "playback.h"

static int check_fname(char* fname);
static void __parse(char* const tokens[], int count);

FATFS g_fs; /* FatFs文件系统对象 */

#if CMD_CRC32
static void crc32_init(void)
{
    CRC_DeInit();
    CRC_ResetDR();
    CRC_ReverseInputDataSelect(CRC_ReverseInputData_8bits);
    CRC_ReverseOutputDataCmd(ENABLE);
}

static void crc32(const char* msg, int size)
{
    for(register int i = 0; i < size; i++)
    CRC_CalcCRC8bits(msg[i]);
}
#endif

#if CMD_INFO
void FSCMD_info(void)
{
    FRESULT res = res;
    DWORD fre_clst;
    FATFS* fs;
    size_t ssize = ssize;
    res = f_getfree("/", &fre_clst, &fs);

#if FF_MAX_SS != FF_MIN_SS
    ssize = t.ssize;
#else
    ssize = FF_MIN_SS;
#endif

    /* Get total sectors and free sectors */
    int tot_sect = (fs->n_fatent - 2) * fs->csize;
    int fre_sect = fre_clst * fs->csize;

//    uprintf(USART1, "%d %d\n", fs->n_fatent, fs->csize);
    /* Print the free space (assuming 4096 bytes/sector) */
    uprintf(USART1, "%10lu KiB total drive space.\n%10lu KiB available.\n",
        tot_sect * 4, fre_sect * 4);
}
#endif

#if CMD_CAT
void FSCMD_cat(char* fname)
{
    FIL fil;
    FRESULT res;

    res = f_open(&fil, fname, FA_READ);
    if(res) {
        uprintf(USART1, "Failed to open file.\n");
        return;
    }
    unsigned char buf[16];

    while(1) {
        unsigned int size;
        f_read(&fil, buf, 16, &size);
        if(size == 0)
            return;
        USART_WriteData_s(USART1, buf, size);
    }
    f_close(&fil);
    uprintf(USART1, "\n");
}
#endif

#if CMD_LL
void FSCMD_ll(void)
{
    DIR dir;
    FRESULT res = res;

    FILINFO finfo;
    res = f_opendir(&dir, "/");
    uprintf(USART1, "%d\n", res);
    int n = 512;
    while(f_readdir(&dir, &finfo) == FR_OK && n--) {
        if(finfo.fname[0])
            uprintf(USART1, "%8d    %-12s\n", finfo.fsize, finfo.fname);
    }
    f_closedir(&dir);
}
#endif

#if CMD_STATUS
void FSCMD_status(void)
{
    uprintf(USART1, "%02x\n", SPIFLASH_ReadStatus());
}
#endif

#if CMD_LS
void FSCMD_ls(void)
{
    DIR dir;
    FRESULT res = res;
    FILINFO finfo;
    res = f_opendir(&dir, "/");
    if(res) {
        uprintf(USART1, "Failed to opendir. %d\n", res);
        return;
    }
    res = f_findfirst(&dir, &finfo, "", "*");
    while(res == FR_OK && finfo.fname[0]) {
        uprintf(USART1, "%s\n", finfo.fname);
        res = f_findnext(&dir, &finfo);
    }
    if(res) {
        uprintf(USART1, "Failed to opendir. %d\n", res);
        return;
    }
}
#endif

#if CMD_RM
void FSCMD_rm(char* fname)
{
    FRESULT res = f_unlink(fname);
    uprintf(USART1, "%d\n", res);
}
#endif

#if CMD_FLASHID
void FSCMD_flashid(void)
{
    SPIFLASH_Info_t* info = SPIFLASH_ReadInfo();
    uprintf(USART1, "%02x %02x %02x %02x\n", info->capacity, info->device_id,
        info->manufacturer_id, info->memory_type);
}
#endif

#if CMD_VIEWSECT
void FSCMD_viewsect(int sectnum)
{
    unsigned char buf[4096];
    unsigned short sum = 0;

    SPIFLASH_Read(sectnum * 4096, 4096, buf);
    for(int i = 0; i < 4096; i++) {
#if 1
        if(i % 32 == 0)
            uprintf(USART1, "%03x: ", i);
        uprintf(USART1, "%02x ", buf[i]);
        if(i % 32 == 31)
            uprintf(USART1, "\n");
#endif
        sum += buf[i];
    }
    uprintf(USART1, "sector %d checksum: %04x\n", sectnum, sum);
}
#endif

#if CMD_CHECK
void FSCMD_check(void)
{
    int ret = SPIFFS_check(&g_fs);
    SPIFFS_CHECK(ret);
    printf("OK\n");
}
#endif

#if CMD_MV
void FSCMD_mv(char* fname, char* fname2)
{
    if(!SPIFFS_mounted(&g_fs))
    return;
    spiffs_file fd = SPIFFS_open(&g_fs, fname, SPIFFS_RDWR, 0);
    if(fd < 0) {
        printf("Failed to open file %s\n", fname);
        return;
    }
    SPIFFS_close(&g_fs, fd);
    if(check_fname(fname2) < 0)
    return;

    int ret = SPIFFS_rename(&g_fs, fname, fname2);
    SPIFFS_CHECK(ret);
    printf("OK\n");
}
#endif

#if CMD_MKFS
void FSCMD_mkfs(void)
{
    SPIFLASH_ChipErase();
    unsigned char workbuf[4096];
    FRESULT res = f_mkfs("0:", FM_FAT | FM_SFD, 4096, workbuf, 4096);
    uprintf(USART1, "%d\n", res);

    res = f_mount(&g_fs, "0:", 1);
    uprintf(USART1, "%d\n", res);
}
#endif

static int check_fname(char* fname)
{
    if(strlen(fname) < 1) {
        printf("Filename too short.\n");
        return -1;
    }
    else if(strlen(fname) > 16) {
        printf("Filename too long.\n");
        return -2;
    }
    int i;
    for(i = 0; i < strlen(fname); i++) {
        int c = fname[i];
        if(!isalpha(c) && !isdigit(c) && (c != '_') && (c != '-')
            && (c != '.')) {
            printf(
                "Filename should contain only alphanumeric characters or dots or underscores.\n");
            return -3;
        }
    }
    return 0;
}

#if CMD_TOUCH
void FSCMD_touch(char* fname)
{
    FIL fil;

    if(check_fname(fname) < 0)
        return;
    f_open(&fil, fname, FA_CREATE_NEW | FA_WRITE);
    f_close(&fil);
}
#endif

#if CMD_CRC32
void FSCMD_crc32(char* fname)
{
    char buf[128];
    register int n;

    spiffs_file fd = SPIFFS_open(&g_fs, fname, SPIFFS_RDWR, 0);
    SPIFFS_CHECK(fd);

    crc32_init();
    while((n = SPIFFS_read(&g_fs, fd, buf, 128)) > 0) {
        crc32(buf, n);
    }
    printf("0x%08lx\n", ~CRC_GetCRC());
    SPIFFS_close(&g_fs, fd);
}
#endif

#if CMD_L
void FSCMD_l(char* fname)
{
    ILI9325_Clear(0x00);
    load_jpg(fname);
}
#endif

void FSCMD_Parse(char* msg)
{
    char *tokens[16], *token;
    char seps[] = "?# ,";
    int count = 0;

    token = strtok(msg, seps);
    while(token != NULL) {
        tokens[count] = token;
        count++;
        token = strtok(NULL, seps); // Get next token:
    }
    __parse(tokens, count);
}

static void __parse(char* const tokens[], int count)
{
    if(0) {
    }
#if CMD_LS
    else if(strcmp(tokens[0], "ls") == 0 && count == 1)
        FSCMD_ls();
#endif
#if CMD_LL
    else if(strcmp(tokens[0], "ll") == 0 && count == 1) {
        FSCMD_ll();
    }
#endif
#if CMD_TOUCH
    else if(strcmp(tokens[0], "touch") == 0 && count >= 2) {
        for(int i = 1; i < count; i++)
            FSCMD_touch(tokens[i]);
    }
#endif
#if CMD_RM
    else if(strcmp(tokens[0], "rm") == 0 && count == 2)
        FSCMD_rm(tokens[1]);
#endif
#if CMD_FORMAT
    else if(strcmp(tokens[0], "format") == 0 && count == 1)
    FSCMD_format();
#endif
#if CMD_INITIALIZE
    else if(strcmp(tokens[0], "initialize") == 0 && count == 1)
    FSCMD_initialize();
#endif
#if CMD_INFO
    else if(strcmp(tokens[0], "info") == 0 && count == 1)
        FSCMD_info();
#endif
#if CMD_VIEWSECT
    else if(strcmp(tokens[0], "viewsect") == 0 && count == 2)
        FSCMD_viewsect(atoi(tokens[1]));
#endif
#if CMD_MKFS
    else if(strcmp(tokens[0], "mkfs") == 0 && count == 1)
        FSCMD_mkfs();
#endif
#if CMD_STATUS
    else if(strcmp(tokens[0], "status") == 0 && count == 1)
        FSCMD_status();
#endif
#if CMD_CHECK
    else if(strcmp(tokens[0], "check") == 0 && count == 1)
    FSCMD_check();
#endif
#if CMD_CAT
    else if(strcmp(tokens[0], "cat") == 0 && count == 2)
        FSCMD_cat(tokens[1]);
#endif
#if CMD_PLAY
    else if(strcmp(tokens[0], "play") == 0 && count == 2)
        PLAYBACK_Play(tokens[1]);
    else if(strcmp(tokens[0], "stop") == 0 && count == 1)
        PLAYBACK_Stop();
#endif
#if CMD_CRC32
    else if(strcmp(tokens[0], "crc32") == 0 && count == 2)
    FSCMD_crc32(tokens[1]);
#endif
#if CMD_L
    else if(strcmp(tokens[0], "l") == 0 && count == 2)
    FSCMD_l(tokens[1]);
#endif

#if CMD_MV
    else if(strcmp(tokens[0], "mv") == 0 && count == 3)
    FSCMD_mv(tokens[1], tokens[2]);
#endif
#if CMD_FLASHID
    else if(strcmp(tokens[0], "flashid") == 0 && count == 1)
        FSCMD_flashid();
#endif
}

void foo(void)
{
    static char buf[16];
    check_fname(buf);
}
