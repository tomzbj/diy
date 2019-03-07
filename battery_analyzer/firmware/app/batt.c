#include <stdio.h> 
#include "adc_f1.h"
#include "dac_f1.h"
#include "batt.h"

void BATT_ViewVoltage(void)
{
    printf("%.3fV\n", ADC_GetResult()->volt);
}

void BATT_Measure(const char* msg)
{
    int curr, v_end, volt;
    float q = 0;

    sscanf(msg + 1, "%d%d", &curr, &v_end);
    printf("Load current[mA] = %d\n", curr);
    printf("Ending voltage[mV] = %d\n", v_end);

    DAC_CurrentSet(curr);

    do {
        int inteval = 1000;
        volt = ADC_GetResult()->volt * 1000.0;

        q += curr * (inteval / 1000.0) / 3600.0;
        printf("%6.3f %6d\n", q, volt);
        _delay_ms(1000);
    }while(volt > v_end);
    DAC_CurrentSet(0);
}

void BATT_Parse(const char* msg, int size)
{
    switch(msg[0]) {
        case 'v':
        case 'V':
            BATT_ViewVoltage();
            break;
        case 'm':
        case 'M':
            BATT_Measure(msg);
            break;
        case 'o':
        case 'O': {
            int curr;
            sscanf(msg + 1, "%d", &curr);
            printf("%d\n", curr);
            DAC_CurrentSet(curr);
            break;
        }
    }
}

