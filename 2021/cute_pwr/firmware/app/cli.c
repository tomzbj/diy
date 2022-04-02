#include "misc.h"
#include "platform.h"

#define MSG_LEN 128

static void ParseRD(char* const tokens[], int count)
{
    if(0) {
    }
    else if(strcasecmp(tokens[1], "VOLT") == 0) {
        printf("%lu\n", ADC1->DR);
    }
    else if(strcasecmp(tokens[1], "ADC") == 0) {
    }
    else if(strcasecmp(tokens[1], "ALL1") == 0) {
        static int count;
        count++;
        count %= 5;
        DataConfig_t* pdc = DC_Get();
        if(count == 0)
            printf("|%8s|%6s|%6s|%8s|%8s|%6s|%8s|%8s|%6s|\n", //
                "SETPOINT", "V_DCDC", "V_OUT", "I_OUT/mA", //
                "MEASURE", "V_OUT", "I_OUT/mA", "P_OUT", "V_BAT");
        printf(
            "|%8s|%6.3f|%6.3f|%8.3f|%8s|%6.3f|%8.3f|%8.3f|%6.3f|\n", //
            "", pdc->status.v_dcdc, pdc->status.v_sp, pdc->status.i_sp * 1000, //
            "", pdc->data.vout, pdc->data.iout * 1000.0, //
            pdc->data.vout * pdc->data.iout, pdc->data.vbat);
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
    else if(strcasecmp(tokens[1], "v_dcdc") == 0) {
        float v_dcdc = strtod(tokens[2], NULL);
        if(v_dcdc < 15.0 && v_dcdc > 1.24)
            pdc->status.v_dcdc = v_dcdc;
        ok();
    }
    else if(strcasecmp(tokens[1], "v_sp") == 0) {
        float v_sp = strtod(tokens[2], NULL);
        if(v_sp < 12.0 && v_sp > 0) {
            pdc->status.v_sp = v_sp;
            pdc->status.v_dcdc = v_sp + 2;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "i_sp") == 0) {
        float i_sp = strtod(tokens[2], NULL);
        if(i_sp < 12.0 && i_sp > 0) {
            pdc->status.i_sp = i_sp;
        }
        ok();
    }
    else if(strcasecmp(tokens[1], "dac1") == 0) {
        unsigned short dac = atoi(tokens[2]);
        DAC_SetChannel1Data(DAC_Align_12b_R, dac);
        ok();
    }
    else if(strcasecmp(tokens[1], "dac2") == 0) {
        unsigned short dac = atoi(tokens[2]);
        DAC_SetChannel2Data(DAC_Align_12b_R, dac);
        ok();
    }
    else if(strcasecmp(tokens[1], "pwm") == 0) {
        unsigned short pwm = atoi(tokens[2]);
        TIM_SetCompare4(TIM2, pwm);
        ok();
    }
    else if(strcasecmp(tokens[1], "pwm2") == 0) {
        unsigned short pwm = atoi(tokens[2]);
        TIM_SetCompare4(TIM1, pwm);
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
        unsigned long c = strtol(tokens[1], NULL, 16);
        GUI_SetBkColor(c);

        int t1 = DWT->CYCCNT;
        GUI_Clear();
//        ST7735_Clear(c);
        int t2 = DWT->CYCCNT;
        printf("%d\n", t2 - t1);
        ok();
    }
}

static void ParseBin(const unsigned char* msg, int size)
{
}

void CLI_Parse(const unsigned char* msg, int size)
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
