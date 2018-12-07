#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "misc.h"
#include "adc_f0.h" 
#include "usart.h"
#include "pid.h"

#define MAX_SAMPLES 64

unsigned short g_adc_buffer[MAX_SAMPLES];
volatile float g_adc;


typedef enum {
    NIMH, LFP
} BattTypeTypeDef;

BattStatusTypeDef g_status = BATT_ERR;

const char* BattStatusString[] = {"BATT_ERR", "BATT_PRE_CHARGE",
    "BATT_FAST_CHARGE", "BATT_TICKLE_CHARGE", "BATT_FULL"};

BattStatusTypeDef ADC_GetBattStatus(void)
{
    return g_status;
}

void ADC_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    // Internal ADC for VSEN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_DeInit(DMA1_Channel1);

    // 默认: 外设基址0, 内存基址0, 方向为外设源, 缓存大小0, 缓存自增disable
    // 内存自增disable, 外设数据大小byte, 内存数据大小byte, 模式normal
    // 优先级low, 内存到内存disable
    DMA_StructInit(&DMA_InitStructure);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(g_adc_buffer);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = MAX_SAMPLES;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_ClearITPendingBit(DMA_IT_TC);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    _dbg();

    ADC_DeInit(ADC1);
    RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
    ADC_StructInit(&ADC_InitStructure);
    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_239_5Cycles);
    ADC_GetCalibrationFactor(ADC1);
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN))
        ;
    ADC_StartOfConversion(ADC1);
}

int func_comp(unsigned short* a, unsigned short* b)
{
    return (*a) < (*b);
}

void ADCDMA_Conv_Complete_IRQHandler(void)
{
    static float sum_volt = 0, sum_curr = 0;
    static int count = 0;
    for(int i = 0; i < MAX_SAMPLES; i += 2) {
        sum_volt += g_adc_buffer[i];
        sum_curr += g_adc_buffer[i + 1];
    }
    float volt = sum_volt / MAX_SAMPLES / 4096.0 * 3.30 * 2.019 * 2;
    float curr = sum_curr / MAX_SAMPLES / 4096.0 * 3.30 * 2;
    sum_volt = 0;
    sum_curr = 0;

    BattTypeTypeDef type;

    if(volt - curr < 1.8)
        type = NIMH;
    else
        type = LFP;

#if 1
    if(count == 0) {
        TIM_SetCompare1(TIM14, 0);      // shutdown dc-dc
    }
    else if(count == 80) {
        if(type == NIMH) {
            if(volt < 0.1)
                g_status = BATT_ERR;
            else if(volt < 0.8)
                g_status = BATT_PRE_CHARGE;
            else if(volt < 1.3)
                g_status = BATT_FAST_CHARGE;
            else if(g_status != BATT_FULL) {
                g_status = BATT_TICKLE_CHARGE;
            }
        }
        else {
            if(volt < 2.4)
                g_status = BATT_PRE_CHARGE;
            else if(volt < 3.5) {
                g_status = BATT_FAST_CHARGE;
            }
            else if(g_status != BATT_FULL) {
                g_status = BATT_TICKLE_CHARGE;
            }
        }
    }
    else if(count > 100) {
        if(g_status == BATT_ERR)
            TIM_SetCompare1(TIM14, 0);      // shutdown dc-dc
        else if(g_status == BATT_PRE_CHARGE)
            PID_Adjust(0.025, curr);
        else if(g_status == BATT_FAST_CHARGE)
            PID_Adjust(0.200, curr);
        else if(g_status == BATT_TICKLE_CHARGE && type == NIMH)
            PID_Adjust(1.430, volt - curr);
        else if(g_status == BATT_TICKLE_CHARGE && type == LFP)
            PID_Adjust(3.650, volt - curr);
        else if(g_status == BATT_FULL && type == NIMH)
            PID_Adjust(1.40, volt);
        else if(g_status == BATT_FULL && type == LFP)
            PID_Adjust(3.60, volt);
    }
    if(count > 1950) {
        if(g_status == BATT_TICKLE_CHARGE && curr < 0.025 && type == NIMH)
            g_status = BATT_FULL;
        if(g_status == BATT_TICKLE_CHARGE && curr < 0.025 && type == LFP)
            g_status = BATT_FULL;
    }
#endif
//    PID_Adjust(0.1, curr);
    if(count == 1990) {
        uprintf(USART1, "1990: status=%s volt=%.3f curr=%.3f duty=%d\n",
                BattStatusString[g_status], volt, curr, TIM_GetCapture1(TIM14));
    }
    count++;
    count %= 2000;
}

float ADC_Read(void)
{
    return g_adc;
}
