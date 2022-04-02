#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "misc.h"
#include "platform.h"

#define MSG_LEN 128

#define ok() printf("ok.\n")

extern FATFS g_fs;

#define chkret(expr) ((expr == 0) ? chk_ok() : chk_failed(expr, (char*)__FILE__, __LINE__))
#define chk_failed(x, file, line) ({printf("Err %d %s line %d\n", x, file, line); return;})
#define chk_ok() (void)0

//static int check_fname(const char* fname);

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
    int tot_k = (fs->n_fatent - 2) * fs->csize * ssize / 1024;
    int fre_k = fre_clst * fs->csize * ssize / 1024;
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

static void cmd_wavinf(const char* tokens[])
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

static void ParseRD(char* const tokens[], int count)
{
//    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "zt_ntasks") == 0) {
        printf("%d\n", zt_get_num_tasks());
    }
    else if(strcasecmp(tokens[1], "test") == 0) {
        unsigned char buf[512];
        bzero(buf, sizeof(buf));
        int t1, t2;
        t1 = DWT->CYCCNT;
        int ret = SD_ReadBlock(buf, 0, sizeof(buf));
        t2 = DWT->CYCCNT;

        printf("ret: %d\n", ret);
        const int batch = 16;
        if(ret == SD_OK) {
            for(int i = 0; i < batch; i++) {
                if(i % batch == 0)
                    printf("%04X:  ", i);
                printf("%02X ", buf[i]);
                if((i + 1) % batch == 0)
                    printf("\n");
            }
            for(int i = sizeof(buf) - batch; i < sizeof(buf); i++) {
                if(i % batch == 0)
                    printf("%04X:  ", i);
                printf("%02X ", buf[i]);
                if((i + 1) % batch == 0)
                    printf("\n");
            }
            printf("cycles: %d\n", t2 - t1);
        }
    }
    else if(strcasecmp(tokens[1], "test2") == 0) {
        unsigned char buf[10240];
        bzero(buf, sizeof(buf));
        int t1, t2;
        t1 = DWT->CYCCNT;
        int ret = SD_ReadMultiBlocks(buf, 0, 512, 20);
        t2 = DWT->CYCCNT;

        printf("ret: %d\n", ret);
        const int batch = 16;
        if(ret == SD_OK) {
            for(int i = 0; i < sizeof(buf); i++) {
                if(i % batch == 0)
                    printf("%04X:  ", i);
                printf("%02X ", buf[i]);
                if((i + 1) % batch == 0)
                    printf("\n");
            }
            printf("cycles: %d\n", t2 - t1);
        }
    }
}

static void ParseWR(char* const tokens[], int count)
{
//    DataConfig_t* pdc = DC_Get();

    if(0) {
    }
    else if(strcasecmp(tokens[1], "zt_start") == 0) {
        int tid = strtol(tokens[2], NULL, 10);
        zt_start(tid);
        ok();
    }
    else if(strcasecmp(tokens[1], "zt_stop") == 0) {
        int tid = strtol(tokens[2], NULL, 10);
        zt_stop(tid);
        ok();
    }
    else if(strcasecmp(tokens[1], "i2s") == 0) {
        unsigned short val = strtol(tokens[2], NULL, 10);
//        SPI_I2S_SendData(SPI2, val);
        I2S_Set(val);
        ok();
    }
    else if(strcasecmp(tokens[1], "play") == 0) {
        char* fname = tokens[2];
        char* suffix = strstr(fname, ".");
        if(strncasecmp(suffix, ".wav", 4) == 0)
            play_wav_init(tokens[2]);
        else if(strncasecmp(suffix, ".mp3", 4) == 0)
            play_mp3_init(tokens[2]);
        zplay_start();
        ok();
    }
    else if(strcasecmp(tokens[1], "testread") == 0) {
           char* fname = tokens[2];
           FIL f;
           unsigned int n;
           unsigned char buf[2048];
           f_open(&f, fname, FA_READ);
           int t1=DWT->CYCCNT;
           f_read(&f, buf, sizeof(buf), &n);
           int t2=DWT->CYCCNT;
           f_close(&f);
           printf("%d\n", t2-t1);
           ok();
    }
    else if(strcasecmp(tokens[1], "stop") == 0) {
        zplay_stop();
        ok();
    }
}

static void Parse(const char* tokens[], int count)
{
    if(strcasecmp(tokens[0], "TEST") == 0)
        printf("TEST OK\n");
    else if(strcasecmp(tokens[0], "REBOOT") == 0) {
        printf("Done.\n");
        NVIC_SystemReset();
    }
    else {
        const struct {
            void (*func)(const char*[]);
            const char* name;
            int n_args;
        } cmds[] = {
            {cmd_ls, "ls", 1},
            {cmd_ls, "ll", 1},
            {cmd_cat, "cat", 2},
            {cmd_hexview, "hexview", 2},
            {cmd_rm, "rm", 2},
            {cmd_wavinf, "wavinf", 2},
            {cmd_mv, "mv", 3},
            //              {cmd_format, "format", 1},
//              {cmd_viewindexrec, "viewindexrec", 2},
//              {cmd_convertindextobin, "conv", 2},
//              {cmd_crc32, "crc32", 2},
//              {cmd_crc32_soft, "crc32soft", 2},
//              {cmd_touch, "touch", 2},
            {cmd_info, "info", 1},
        //              {cmd_viewsyscfg, "viewsyscfg", 1},
//              {cmd_setrrefl, "setrrefl", 2},
//              {cmd_setrrefh, "setrrefh", 2},
//              {cmd_setdefaults, "setdefaults", 1},
            };

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
}

void CLI_Parse(const unsigned char* msg, int size)    //, int type)
{
    char* tokens[8], * token;
    char seps[] = "? ,#", string[MSG_LEN];
    int len, count = 0;

    bzero(string, MSG_LEN);
    strncpy(string, (const char*)msg, size);

    len = strlen(string);
    while(string[len - 1] == '\n' || string[len - 1] == '\r') {
        string[len - 1] = '\0';
        len--;
    }

    token = strtok(string, seps);
    while(token != NULL) {
        tokens[count] = token;
        token = strtok(NULL, seps);    // Get next token:
        count++;
        if(count >= 8)
            break;
    }
    if(count == 0)
        return;

    if(strcasecmp(tokens[0], "RD") == 0) {
        ParseRD(tokens, count);
    }
    else if(strcasecmp(tokens[0], "WR") == 0) {
        ParseWR(tokens, count);
    }
    else
        Parse((const char**)tokens, count);
}
