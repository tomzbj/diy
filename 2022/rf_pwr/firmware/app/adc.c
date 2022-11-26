#include "misc.h"
#include "platform.h"

void ADCDMA_IRQHandler(void)
{
    DataConfig_t* pdc = DC_Get();
    unsigned long sum[DC_ADC_CHANNEL];
    sum[0] = sum[1] = sum[2] = 0;

    for(int i = 0; i < DC_ADC_BUF_SIZE * DC_ADC_CHANNEL; i += DC_ADC_CHANNEL) {
        sum[0] += pdc->data.adc_buf[i];
        sum[1] += pdc->data.adc_buf[i + 1];
        sum[2] += pdc->data.adc_buf[i + 2];
    }
    for(int i = 0; i < DC_ADC_CHANNEL; i++) {
        sum[i] /= DC_ADC_BUF_SIZE;
        pdc->data.cur[i] = sum[i] * 3300.0 / 20.0 / 4095.0 * 10.0; // / sum[3];
    }
}
