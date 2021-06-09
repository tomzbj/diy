#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "misc.h"
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
    else if(strcasecmp(tokens[1], "VOLT") == 0) {
        printf("%lu\n", ADC1->DR);
    }
    else if(strcasecmp(tokens[1], "LCR") == 0) {
        LCR_DoMeasure();
    }
    else if(strcasecmp(tokens[1], "ADC") == 0) {
        for(int i = 0; i < DC_NUM_OF_SAMPLES; i++)
            printf("%lu\n", pdc->data.adc_buf[i]);
    }
}

static void ParseWR(char* const tokens[], int count)
{
//    DataConfig_t* pdc = DC_Get();
    if(0) {
    }
    else if(strcasecmp(tokens[1], "AMP") == 0) {
        int amp = strtol(tokens[2], NULL, 10);
        DAC_SetAmplitude(amp);
        ok();
    }
    else if(strcasecmp(tokens[1], "GAIN") == 0) {
        int id = strtol(tokens[2], NULL, 10);
        switch(id) {
            case 0:
                GPIOB->BRR = GPIO_Pin_12;
                GPIOB->BRR = GPIO_Pin_13;
                break;
            case 1:
                GPIOB->BSRR = GPIO_Pin_12;
                GPIOB->BRR = GPIO_Pin_13;
                break;
            case 2:
                GPIOB->BRR = GPIO_Pin_12;
                GPIOB->BSRR = GPIO_Pin_13;
                break;
            case 3:
                GPIOB->BSRR = GPIO_Pin_12;
                GPIOB->BSRR = GPIO_Pin_13;
                break;
            default:
                break;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "ZT_STOP") == 0) {
        int id = strtol(tokens[2], NULL, 10);
        zt_stop(id);
        ok();
    }
    else if(strcasecmp(tokens[1], "ZT_START") == 0) {
        int id = strtol(tokens[2], NULL, 10);
        zt_start(id);
        ok();
    }
    else if(strcasecmp(tokens[1], "dac") == 0) {
        unsigned short dac1 = strtod(tokens[2], NULL);
        unsigned short dac2 = strtod(tokens[3], NULL);
        DAC_SetDualChannelData(DAC1, DAC_Align_12b_R, dac2, dac1);
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

static void ParseBin(const unsigned char* msg, int size)
{
}

void CLI_Parse(const unsigned char* msg, int size, int type)
{
    char* tokens[8], * token;
    char seps[] = "? ,#", string[MSG_LEN];
    int len, count = 0;

    bzero(string, MSG_LEN);
    strncpy(string, (const char*)msg, size);
//        for(i = 0; i < size; i++) { if(isalpha((int)string[i])) string[i] = toupper((int)string[i]); }

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
        Parse(tokens, count);
}
