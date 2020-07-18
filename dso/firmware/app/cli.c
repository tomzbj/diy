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
    if(0) {
    }
    else if(strcasecmp(tokens[1], "DRAWWAVE") == 0) {
        int t1 = DWT->CYCCNT;
        UI_DrawTrace();
        int t2 = DWT->CYCCNT;
        printf("%d\n", t2 - t1);
    }
    else if(strcasecmp(tokens[1], "VOLT") == 0) {
        printf("%lu\n", ADC1->DR);
    }
    else if(strcasecmp(tokens[1], "DATA") == 0) {
        int start = strtol(tokens[2], NULL, 10);
        int num = strtol(tokens[3], NULL, 10);
        int size = 1000;
        unsigned short buf[1000];
        while(num > 0) {
            if(num < size)
                size = num;
//            SPISRAM_FastRead(start, size * sizeof(buf[0]), buf);
            num -= size;
            start += size * sizeof(buf[0]);
            for(int i = 0; i < size; i++) {
                printf("%u\n", (buf[i] * 3300) >> 12);
            }
        }
    }
    else if(strcasecmp(tokens[1], "ADC") == 0) {
        DataConfig_t* pdc = DC_Get();
        DMA_Cmd(DMA1_Channel1, DISABLE);
        for(int i = 0; i < DC_ADC_BUF_SIZE; i++) {
            printf("%d\n", pdc->data.buf[i]);
        }
        DMA_Cmd(DMA1_Channel1, ENABLE);
    }
}

static void ParseWR(char* const tokens[], int count)
{
    DataConfig_t* pdc = DC_Get();

    if(0) {
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
    else if(strcasecmp(tokens[1], "PB1") == 0) {
        if(strcasecmp(tokens[2], "ON") == 0) {
            GPIOB->BSRR = GPIO_Pin_1;
        }
        else if(strcasecmp(tokens[2], "OFF") == 0) {
            GPIOB->BRR = GPIO_Pin_1;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "PB8") == 0) {
        if(strcasecmp(tokens[2], "ON") == 0) {
            GPIOB->BSRR = GPIO_Pin_8;
        }
        else if(strcasecmp(tokens[2], "OFF") == 0) {
            GPIOB->BRR = GPIO_Pin_8;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "PB11") == 0) {
        if(strcasecmp(tokens[2], "ON") == 0) {
            GPIOB->BSRR = GPIO_Pin_11;
        }
        else if(strcasecmp(tokens[2], "OFF") == 0) {
            GPIOB->BRR = GPIO_Pin_11;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "HC595") == 0) {
        unsigned char val = strtol(tokens[2], NULL, 16);
        HC595_WriteByte(val);
        printf("%02x\n", val);
    }

    else if(strcasecmp(tokens[1], "COUPLING") == 0) {
        if(strcasecmp(tokens[2], "DC") == 0) {
            pdc->cfg.coupling = DC_COUPLING_DC;
        }
        else if(strcasecmp(tokens[2], "AC") == 0) {
            pdc->cfg.coupling = DC_COUPLING_AC;
        }
        else if(strcasecmp(tokens[2], "GND") == 0) {
            pdc->cfg.coupling = DC_COUPLING_GND;
        }
    }
    else if(strcasecmp(tokens[1], "TRIG_LEVEL") == 0) {
        signed short val = strtol(tokens[2], NULL, 10);
        pdc->cfg.trig_level = val;
    }
    else if(strcasecmp(tokens[1], "TRIG_MODE") == 0) {
        if(strcasecmp(tokens[2], "AUTO") == 0) {
            pdc->cfg.trig_mode = DC_TRIG_MODE_AUTO;
        }
        else if(strcasecmp(tokens[2], "NORMAL") == 0) {
            pdc->cfg.trig_mode = DC_TRIG_MODE_NORMAL;
        }
        else if(strcasecmp(tokens[2], "SINGLE") == 0) {
            pdc->cfg.trig_mode = DC_TRIG_MODE_SINGLE;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "TRIG_EDGE") == 0) {
        if(strcasecmp(tokens[2], "RISING") == 0) {
            pdc->cfg.trig_edge = DC_TRIG_EDGE_RISING;
        }
        else if(strcasecmp(tokens[2], "FALLING") == 0) {
            pdc->cfg.trig_edge = DC_TRIG_EDGE_FALLING;
        }
        else if(strcasecmp(tokens[2], "BOTH") == 0) {
            pdc->cfg.trig_edge = DC_TRIG_EDGE_BOTH;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "TIME_BASE") == 0) {
        unsigned short val = strtol(tokens[2], NULL, 10);
        pdc->cfg.timebase = val;
        ok();
    }
    else if(strcasecmp(tokens[1], "LABEL") == 0) {
        unsigned char id = strtol(tokens[2], NULL, 10);
        UI_SetLabelText(id, tokens[3]);
        ok();
    }
    else if(strcasecmp(tokens[1], "TIM6") == 0) {
        unsigned short val = strtol(tokens[2], NULL, 10);
        TIM_SetAutoreload(TIM6, val - 1);
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
    else if(strcasecmp(tokens[0], "CLEAR") == 0) {
        unsigned short c = strtol(tokens[1], NULL, 16);
        int t1 = DWT->CYCCNT;
        UG_FillScreen(c);
        int t2 = DWT->CYCCNT;
        printf("%d\n", t2 - t1);
        ok();
    }
}

static void ParseBin(const unsigned char* msg, int size)
{
}

void CLI_Parse(const unsigned char* msg, int size, int type)
{
    if(type == CLI_TYPE_ASCII) {
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
    else if(type == CLI_TYPE_BINARY) {
        if(size < 4)        // msg至少需要4字节
            return;
        if(CRC16_CheckIntegrity(msg, size) != CRC_OK)
            return;
        ParseBin(msg, size);
    }
}
