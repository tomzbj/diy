#include "misc.h"
#include "dataconfig.h"

void ADC_Config(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
    ADC1_DeInit();
    ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_6,
        ADC1_PRESSEL_FCPU_D18, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT,
        ADC1_SCHMITTTRIG_CHANNEL6, DISABLE);
    ADC1_Cmd(ENABLE);
    ADC1_StartConversion();
}

void ADC_Poll(void)
{
    if(ADC1_GetFlagStatus(ADC1_FLAG_EOC) != RESET) {
        DataConfig_t* pdc = DC_Get();
        ADC1_ClearFlag(ADC1_FLAG_EOC);
        pdc->data.data = ADC1_GetConversionValue();
    }
}
