#include "misc.h"
#include "platform.h"
#include <math.h>

typedef struct {
    unsigned short i, v;
} sample_t;

static void sample(void)
{
    __disable_irq();
    DMA_Cmd(DMA1_Channel1, ENABLE);
//    int t1 = DWT->CYCCNT;
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
//    int t2 = DWT->CYCCNT;
    __enable_irq();
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_ClearFlag(DMA1_FLAG_TC1);
//    printf("%d\n", sizeof(pdc->data.adc_buf));
//    printf("t: %d\n", t2 - t1);
}

void LCR_DoMeasure(void)
{
    DataConfig_t* pdc = DC_Get();
    sample_t* pbuf = (sample_t*)pdc->data.adc_buf;

    float mean_v = 0, mean_i = 0, prod_i = 0, prod_q = 0, phase;
    float abs_v = 0, abs_i = 0, z, x, r;

    sample();

    int buf_size = sizeof(pdc->data.adc_buf) / sizeof(pdc->data.adc_buf[0]);
    for(int i = 0; i < buf_size; i++) {
        mean_v += pbuf[i].v;
        mean_i += pbuf[i].i;
    }
    mean_v /= buf_size;
    mean_i /= buf_size;
//    printf("%f %f\n", mean_v, mean_i);

    for(int i = 50; i < buf_size - 50; i++) {
        prod_i += -(pbuf[i].v - mean_v) * (pbuf[i].i - mean_i);
        prod_q += (pbuf[i + 25].v - mean_v) * (pbuf[i].i - mean_i);
        abs_v += fabs(pbuf[i].v - mean_v);
        abs_i += fabs(pbuf[i].i - mean_i);
    }
    prod_i /= buf_size;
    prod_q /= buf_size;
    phase = atan2(prod_q, prod_i);
    z = abs_v / abs_i;
    r = 100 * z * cos(phase);
    x = 100 * z * sin(phase);

//    printf("%f %f\n", prod_i, prod_q);
    printf("%f\n", phase * 180 / 3.141593);
    if(phase >= 0)
        printf("%f R + %.4e H\n", r, x / 2.0 / 3.141593 / 10e3);
    else
        printf("%f R + %.4e F\n", r, -1 / x / 2.0 / 3.141593 / 10e3);
}
