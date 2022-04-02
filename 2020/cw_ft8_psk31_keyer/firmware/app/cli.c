#include <string.h>
#include "platform.h"

#define MSG_LEN 128

typedef enum {
    CRC_OK = 0, CRC_MISMATCH = -1
} CRC_Status;

static unsigned short CRC16_Calc(const unsigned char* msg, int size)
{
    unsigned short crc = 0xffff;
    for(int i = 0; i < size; i++) {
        crc ^= msg[i];
        for(int j = 0; j < 8; j++) {
            bool lsb = crc % 2;
            crc >>= 1;
            if(lsb)
                crc ^= 0xa001;
        }
    }
    return crc;
}

CRC_Status CRC16_CheckIntegrity(const unsigned char* msg, int size)
{
    unsigned short crc, crc2;
    crc = CRC16_Calc(msg, size - 2);
    memcpy(&crc2, &msg[size - 2], 2);
    if(crc == crc2)
        return CRC_OK;
    else
        return CRC_MISMATCH;
}

static void ParseRD(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "ms") == 0) {
        printf("%lu\n", pdc->data.ms);
    }
    else if(strcasecmp(tokens[1], "ft8_conv") == 0) {
        unsigned char symbols[100];
        memset(symbols, '\0', 100);
        ft8_encode(tokens[2], symbols);

        int n = sizeof(symbols);
        for(int i = n - 1; i >= 0; i--) {
            if(symbols[i] == '\0')
                n--;
            else
                break;
        }
        for(int i = 0; i < n; i++) {
            printf("%d: %02x\t", i, symbols[i]);
            if(i % 8 == 7)
                printf("\n");
        }
        printf("\n");
    }
    else if(strcasecmp(tokens[1], "time") == 0) {
        unsigned long s = pdc->data.ms / 1000;
        printf("%02lu:%02lu:%02lu, %lu\n", (s / 3600 + pdc->cfg.time_zone) % 24,
            (s / 60) % 60, s % 60, pdc->data.ms % 1000);
    }
    else if(strcasecmp(tokens[1], "cfg") == 0) {
        const char* tx_mode_str[] = {"CW", "BPSK31", "FT8"};
        const char* spk_echo_mode_str[] = {"CW", "RAW"};
        const char* cli_echo_mode_str[] = {"CW", "ASCII"};
        printf("--- ALL CONFIG OPTIONS ---\n");
        printf("  SPK_ECHO_STATE:   %s\n",
            (pdc->cfg.spk_echo_state) ? "ON" : "OFF");
        printf("  SPK_ECHO_MODE:    %s\n",
            spk_echo_mode_str[pdc->cfg.spk_echo_mode]);
        printf("  SPK_ECHO_FREQ:    %d\n", pdc->cfg.spk_echo_freq);
        printf("  CLI_ECHO_STATE:   %s\n",
            (pdc->cfg.cli_echo_state) ? "ON" : "OFF");
        printf("  CLI_ECHO_MODE:    %s\n",
            cli_echo_mode_str[pdc->cfg.cli_echo_mode]);
        printf("  PSK_CARRIER_FREQ: %d\n", pdc->cfg.psk_carrier_freq);
        printf("  CW_SPEED(IN WPM): %d\n", pdc->cfg.cw_wpm);
        printf("  TX_MODE:          %s\n", tx_mode_str[pdc->cfg.tx_mode]);
        printf("\n");
    }
}

static void ParseWR(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "timezone") == 0) {
        int timezone = strtol(tokens[2], NULL, 10);
        pdc->cfg.time_zone = timezone;
        ok();
    }
    else if(strcasecmp(tokens[1], "ms") == 0) {
        unsigned long ms = strtoul(tokens[2], NULL, 10);
        pdc->data.ms = ms;
        ok();
    }
    else if(strcasecmp(tokens[1], "tim6_on") == 0) {
        TIM_Cmd(TIM6, ENABLE);
        ok();
    }
    else if(strcasecmp(tokens[1], "tim6_off") == 0) {
        TIM_Cmd(TIM6, DISABLE);
        ok();
    }
    else if(strcasecmp(tokens[1], "setdefaults") == 0) {
        DC_SetDefaults();
        ok();
    }
    else if(strcasecmp(tokens[1], "repeat") == 0) {
        cw_tx_repeat();
        ok();
    }
    else if(strcasecmp(tokens[1], "tx_mode") == 0) {
        if(strcasecmp(tokens[2], "CW") == 0) {
            pdc->cfg.tx_mode = DC_TX_MODE_CW;
        }
        else if(strcasecmp(tokens[2], "bpsk") == 0) {
            pdc->cfg.tx_mode = DC_TX_MODE_BPSK;
        }
        else if(strcasecmp(tokens[2], "ft8") == 0) {
            pdc->cfg.tx_mode = DC_TX_MODE_FT8;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "cli_echo_mode") == 0) {
        if(strcasecmp(tokens[2], "CW") == 0) {
            pdc->cfg.cli_echo_mode = DC_CLI_ECHO_MODE_CW;
        }
        else if(strcasecmp(tokens[2], "ASCII") == 0) {
            pdc->cfg.cli_echo_mode = DC_CLI_ECHO_MODE_ASCII;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "spk_echo_freq") == 0) {
        int freq = strtol(tokens[2], NULL, 10);
        if(freq < 300 || freq > 1000)
            printf("echo freq must be in 300 - 1000 Hz.\n");
        else {
            pdc->cfg.spk_echo_freq = freq;
            TIM_SetAutoreload(TIM6, 48000000UL / 256 / freq - 1);
            ok();
        }
    }
    else if(strcasecmp(tokens[1], "cw_speed") == 0) {
        int speed = strtol(tokens[2], NULL, 10);
        if(speed < 10 || speed > 60)
            printf("cw speed must be in 10 - 60 wpm.\n");
        else {
            pdc->cfg.cw_wpm = speed;
            ok();
        }
    }
    else if(strcasecmp(tokens[1], "psk_baudrate") == 0) {
        int baudrate = strtol(tokens[2], NULL, 10);
        const int baudrates[] = {31, 63, 125};
        int match = 0;
        for(int i = 0; i < sizeof(baudrates) / sizeof(baudrates[0]); i++) {
            if(baudrates[i] == baudrate)
                match = 1;
        }
        if(!match) {
            printf("psk baudrate must be in ");
            for(int i = 0; i < sizeof(baudrates) / sizeof(baudrates[0]); i++) {
                printf("%d, ", baudrates[i]);
            }
            printf("\n");
        }
        else {
            pdc->cfg.psk_baudrate = baudrate;
            ok();
        }
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
    DC_Update();
}

static void Parse(char* const tokens[], int count)
{
    if(strcasecmp(tokens[0], "TEST") == 0)
        printf("TEST OK\n");
    else if(strcasecmp(tokens[0], "REBOOT") == 0) {
        printf("Done.\n");
        NVIC_SystemReset();
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
    if(count <= 0)
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
