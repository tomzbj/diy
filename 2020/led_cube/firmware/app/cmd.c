#include "platform.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "spiflash.h"
#include "cmd.h"
#include "ff.h"
#include "misc.h"

extern FATFS g_fs;

static int check_fname(const char* fname);

#define chkret(x) (x)

static void cmd_info(const char* tokens[])
{
    DWORD fre_clst;
    FATFS* fs;
    size_t ssize = ssize;
    chkret(f_getfree("/", &fre_clst, &fs));

#if FF_MAX_SS != FF_MIN_SS
    ssize = t.ssize;
#else
    ssize = FF_MIN_SS;
#endif

    /* Get total sectors and free sectors */
    int tot_k = (fs->n_fatent - 2) * fs->csize * 4;
    int fre_k = fre_clst * fs->csize * 4;
    printf("%10d K total space.\n%10d K available.\n\n", tot_k, fre_k);
}

//static void my_puts(const char* s) { fputs(s, stdout); }
static void cmd_hexview(const char* tokens[])
{
    FIL f;
    FILINFO inf;
    unsigned char buf[16];
    unsigned int size, n, pos = 0;

    bzero(buf, sizeof(buf));
    const char* fname = tokens[1];

    chkret(f_stat(fname, &inf));
    chkret(f_open(&f, fname, FA_READ));

    size = inf.fsize;

    while(size > 0) {
        chkret(f_read(&f, buf, sizeof(buf), &n));
        if(n <= 0)
            break;
        printf("%04X: ", pos);
        for(int i = 0; i < n; i++) {
            printf("%02X ", buf[i]);
        }
        printf("\n");
        size -= n;
        pos += n;
    }
    printf("\n");
    f_close(&f);
}

static void cmd_cat(const char* tokens[])
{
    FIL f;
    FILINFO inf;
    char buf[32];
    unsigned int size, n;

    bzero(buf, sizeof(buf));
    const char* fname = tokens[1];

    chkret(f_stat(fname, &inf));
    chkret(f_open(&f, fname, FA_READ));

    size = inf.fsize;

    while(size > 0) {
        chkret(f_read(&f, buf, sizeof(buf), &n));
        if(n <= 0)
            break;
        fwrite(buf, n, 1, stdout);
        fflush(stdout);
        size -= n;
    }
    printf("\n");
    f_close(&f);
}

static void cmd_ls(const char* tokens[])
{
    DIR dir;
    FILINFO finfo;
    int n = 512, ll = 0, tot_size = 0;
    if(strcasecmp(tokens[0], "ll") == 0)
        ll = 1;

    chkret(f_opendir(&dir, "/"));
    while(f_readdir(&dir, &finfo) == FR_OK && n--) {
        if(finfo.fname[0]) {
            tot_size += finfo.fsize;
            if(ll)
                printf("%8d    %-12s\n", (int)finfo.fsize, finfo.fname);
            else
                printf("%s\t", finfo.fname);
        }
    }
    printf("\n%10d bytes used.\n", tot_size);
    cmd_info(tokens);
    f_closedir(&dir);
}

static void cmd_rm(const char* tokens[])
{
    chkret(f_unlink(tokens[1]));
    ok();
}

static void cmd_mv(const char* tokens[])
{
    chkret(f_rename(tokens[1], tokens[2]));
    ok();
}

static void cmd_format(const char* tokens[])
{
    unsigned char workbuf[4096];
    chkret(f_mkfs("0:", FM_FAT | FM_SFD, 4096, workbuf, 4096));
    chkret(f_mount(&g_fs, "0:", 1));
    ok();
}

static int check_fname(const char* fname)
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
            printf("Filename should contain only alphanumeric characters "
                "or dots or underscores.\n");
            return -3;
        }
    }
    return 0;
}

static void cmd_touch(const char* tokens[])
{
    const char* fname = tokens[1];
    FIL fp;
    chkret(check_fname(fname));
    chkret(f_open(&fp, fname, FA_WRITE | FA_CREATE_NEW));
    chkret(f_close(&fp));
    ok();
}

static unsigned long crc32_soft(unsigned long crc, const void* msg, int size)
{
    const unsigned long polynormial = 0xedb88320;
    for(int i = 0; i < size; i++) {
        crc ^= *(unsigned char*)(msg + i);
        for(int j = 0; j < 8; j++) {
            int lsb = crc % 2;
            crc >>= 1;
            if(lsb)
                crc ^= polynormial;
        }
    }
    return crc;
}

static void cmd_crc32_soft(const char* tokens[])
{
    const char* fname = tokens[1];
    char buf[128];
    unsigned int n;
    FIL f;

    chkret(f_open(&f, fname, FA_READ));
    unsigned long crc = 0xffffffff;

    while(1) {
        f_read(&f, buf, sizeof(buf), &n);
        if(n <= 0)
            break;
        crc = crc32_soft(crc, buf, n);
    }
    printf("0x%08lX\n", ~crc);
    f_close(&f);
}


void CMD_Parse(char* msg, int size)
{
    static struct {
        void (*func)(const char*[]);
        const char* name;
        int n_args;
    } cmds[] = {
        {cmd_ls, "ls", 1},
        {cmd_ls, "ll", 1},
        {cmd_cat, "cat", 2},
        {cmd_hexview, "hexview", 2},
        {cmd_rm, "rm", 2},
        {cmd_mv, "mv", 3},
        {cmd_format, "format", 1},
        {cmd_touch, "touch", 2},
        {cmd_info, "info", 1},
    };

    const char *tokens[16], *token;
    char seps[] = "$?# ,";
    int count = 0;

    token = strtok(msg, seps);
    while(token != NULL) {
        tokens[count] = token;
        count++;
        token = strtok(NULL, seps); // Get next token:
    }
    if(count <= 0)
        return;
    for(int i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) {
        if(strcasecmp(tokens[0], cmds[i].name) == 0) {
            if(count == cmds[i].n_args)
                cmds[i].func(tokens);
            else {
                printf("Usage: %s ", cmds[i].name);
                for(int j = 1; j < cmds[i].n_args; j++) {
                    printf("arg%d ", j);
                }
                printf("\n");
            }
        }
    }
}
