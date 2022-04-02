#include <ctype.h>
#include "misc.h"
#include "platform.h"
#include <time.h>

#define MSG_LEN 256
#define ok() printf("ok\n");

static void ParseRD(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "freq") == 0) {
//        printf("%d %lu\n", HSE_VALUE, rcu_clock_freq_get(CK_SYS));
        ok();
    }
    else if(strcasecmp(tokens[1], "jedec") == 0) {
//        printf("%08lX\n", sfud_read_jedec());
//        printf("%08lX\n", SPIFLASH_ReadJedecID());
    }
    else if(strcasecmp(tokens[1], "time") == 0) {
        RTC_TimeTypeDef rts;
        RTC_DateTypeDef rds;
        RTC_GetTime(RTC_Format_BIN, &rts);
        RTC_GetDate(RTC_Format_BIN, &rds);
        printf("%02d-%02d-%02d  ", rds.RTC_Year, rds.RTC_Month, rds.RTC_Date);
        printf("%02d:%02d:%02d\n", rts.RTC_Hours, rts.RTC_Minutes,
            rts.RTC_Seconds);
        struct tm tminfo;
        tminfo.tm_hour = rts.RTC_Hours;
        tminfo.tm_min = rts.RTC_Minutes;
        tminfo.tm_sec = rts.RTC_Seconds;
        tminfo.tm_year = rds.RTC_Year;
        tminfo.tm_mon = rds.RTC_Month;
        tminfo.tm_mday = rds.RTC_Date;
        time_t timestamp = mktime(&tminfo);
        printf("%lu\n", timestamp);
    }
    else if(strcasecmp(tokens[1], "read_test") == 0) {
        const sfud_flash* flash = sfud_get_device_table() + 0;
//        printf("%02x %02x %02x\n", flash->chip.mf_id, flash->chip.type_id, flash->chip.capacity_id);
        unsigned long addr = strtoul(tokens[2], NULL, 16);
        unsigned long size = strtoul(tokens[3], NULL, 10);
        size = (size > 8192) ? 8192 : size;
        unsigned char buf[8192];

        unsigned long t1 = DWT->CYCCNT;
        sfud_read(flash, addr, size, buf);
        unsigned long t2 = DWT->CYCCNT;

        printf("cycles:%lu\n", t2 - t1);
        ok();
    }
    else if(strcasecmp(tokens[1], "flash") == 0) {
        const sfud_flash* flash = sfud_get_device_table() + 0;
//        printf("%02x %02x %02x\n", flash->chip.mf_id, flash->chip.type_id, flash->chip.capacity_id);
        unsigned long addr = strtoul(tokens[2], NULL, 16);
        unsigned long size = strtoul(tokens[3], NULL, 10);
        size = (size > 8192) ? 8192 : size;
        unsigned char buf[8192];
//        SPIFLASH_FastRead(addr, size, buf);

        unsigned long t1 = DWT->CYCCNT;
        sfud_read(flash, addr, size, buf);
        unsigned long t2 = DWT->CYCCNT;

        for(int i = 0; i < size; i++) {
            printf("%02x ", buf[i]);
            if(i % 16 == 15)
                printf("\n");
        }
        printf("\n");
        printf("cycles:%lu\n", t2 - t1);

        ok();
    }
    else if(strcasecmp(tokens[1], "all") == 0) {
    }
    else if(strcasecmp(tokens[1], "bkp") == 0) {
        unsigned long data = RTC_ReadBackupRegister(RTC_BKP_DR0);
        printf("%08lX\n", data);
    }
    else if(strcasecmp(tokens[1], "cfg") == 0) {
        //  cw_wpm;  cw_carrier_freq;  ft8_carrier_freq;  echo_volume;  output_volume;  tx_mode;
        const char* tx_mode_str[] = {"CW", "BPSK31", "FT8"};
        static int count = 0;
        count++;
        count %= 5;
        if(count == 0)
            printf(
                "|CW_WPM|CW_CAR.F|FT8_CAR.F|ECHO_VOL|OUT_VOL|DIM|TX MODE|\n");
        printf("| %4d | %6d | %7d | %6d | %5d |%3d|%7s|\n", pdc->cfg.cw_wpm,
            pdc->cfg.cw_carrier_freq, pdc->cfg.ft8_carrier_freq,
            pdc->cfg.echo_volume, pdc->cfg.output_volume, pdc->cfg.dim,
            tx_mode_str[pdc->cfg.tx_mode]);
    }
}

static void ParseWR(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "setdefaults") == 0) {
        DC_SetDefaults();
        ok();
    }
    else if(strcasecmp(tokens[1], "time") == 0) {
        time_t timestamp = strtoul(tokens[2], NULL, 10);
        struct tm tminfo;
        putenv("TZ=GMT-8");
        tzset();
        memcpy(&tminfo, localtime(&timestamp), sizeof(tminfo));

        printf("%02d-%02d-%02d  ", tminfo.tm_year + 1900, tminfo.tm_mon + 1,
            tminfo.tm_mday);
        printf("%02d:%02d:%02d\n", tminfo.tm_hour, tminfo.tm_min,
            tminfo.tm_sec);

        RTC_TimeTypeDef rts;
        RTC_DateTypeDef rds;

        rts.RTC_Hours = tminfo.tm_hour;
        rts.RTC_Minutes = tminfo.tm_min;
        rts.RTC_Seconds = tminfo.tm_sec;
        rds.RTC_Year = tminfo.tm_year - 112;
        rds.RTC_Month = tminfo.tm_mon + 1;
        rds.RTC_Date = tminfo.tm_mday;
        RTC_SetTime(RTC_Format_BIN, &rts);
        RTC_SetDate(RTC_Format_BIN, &rds);

        ok();
    }
    else if(strcasecmp(tokens[1], "brightness") == 0) {
        unsigned long val = strtoul(tokens[2], NULL, 10);
        TIM_SetCompare1(TIM17, val);
        ok();
    }
    else if(strcasecmp(tokens[1], "bkp") == 0) {
        unsigned long data = strtoul(tokens[2], NULL, 16);
        RTC_WriteBackupRegister(RTC_BKP_DR0, data);
        ok();
    }
    else if(strcasecmp(tokens[1], "echo") == 0) {
        if(strcasecmp(tokens[2], "on") == 0) {
            GPIOB->BRR = GPIO_Pin_11;
            ok();
        }
        else if(strcasecmp(tokens[2], "off") == 0) {
            GPIOB->BSRR = GPIO_Pin_11;
            ok();
        }
    }
    else if(strcasecmp(tokens[1], "dac") == 0) {
        unsigned long data = strtoul(tokens[2], NULL, 10);
        DAC1->DHR12L1 = data;
        printf("%lu\n", data);
        ok();
    }
    else if(strcasecmp(tokens[1], "tx_mode") == 0) {
        if(strcasecmp(tokens[2], "CW") == 0) {
            pdc->cfg.tx_mode = DC_TX_MODE_CW;
            ok();
        }
        else if(strcasecmp(tokens[2], "bpsk") == 0) {
            pdc->cfg.tx_mode = DC_TX_MODE_BPSK;
            ok();
        }
        else if(strcasecmp(tokens[2], "ft8") == 0) {
            pdc->cfg.tx_mode = DC_TX_MODE_FT8;
            ok();
        }
        else
            printf("Mode should be CW or BPSK or FT8.\n");
    }
}

static void Parse(char* const tokens[], int count)
{
    if(strcasecmp(tokens[0], "TEST") == 0)
        printf("TEST OK\n");
    else if(strcasecmp(tokens[0], "REBOOT") == 0) {
        printf("System Reboot.\n\n");
        NVIC_SystemReset();
    }
}

void CLI_Parse(const void* pmsg, int size, int source)
{
    char* tokens[8], * token;
    char seps[] = "? ,#\r\n", string[MSG_LEN];
    int i, len, count = 0;
    char* msg = (char*)pmsg;

    bzero(string, MSG_LEN);
    strncpy(string, msg, size);
    for(i = 0; i < size; i++) {
        if(isalpha((int)string[i]))
            string[i] = toupper((int)string[i]);
    }

    len = strlen(string);
    while(string[len - 1] == '\n' || string[len - 1] == '\r') {
        string[len - 1] = '\0';
        len--;
    }

    token = strtok(string, seps);
    while(token != NULL) {

        tokens[count] = token;
        count++;
        token = strtok(NULL, seps);    // Get next token:
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
        Parse(tokens, count);
}
