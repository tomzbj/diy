#include "misc.h"
#include "platform.h"

void TIMER_Config(void)
{
    // TIM3_CH1 as AUDIO_OUT
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    {
            NVIC_InitTypeDef nis;
    nis.NVIC_IRQChannel = TIM3_IRQn;
    nis.NVIC_IRQChannelPriority = 2;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    }
    TIM_TimeBaseInitTypeDef tbs;
    TIM_OCInitTypeDef tois;

    TIM_TimeBaseStructInit(&tbs);
    tbs.TIM_Prescaler = 7;
    tbs.TIM_ClockDivision = TIM_CKD_DIV1;
    tbs.TIM_CounterMode = TIM_CounterMode_Up;
    tbs.TIM_Period = 255;    // 48M / 9 / 256 = 20833, close to 22050
    TIM_TimeBaseInit(TIM3, &tbs);

    TIM_OCStructInit(&tois);
    tois.TIM_OutputState = TIM_OutputState_Enable;
    tois.TIM_Pulse = 128;
    tois.TIM_OCPolarity = TIM_OCPolarity_High;
    tois.TIM_OCIdleState = TIM_OCIdleState_Reset;
    tois.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OC1Init(TIM3, &tois);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_DMAConfig(TIM3, TIM_DMABase_CCR1, TIM_DMABurstLength_1Transfer);
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
//    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM3, DISABLE);    // TIM3 enable counter
//    TIM_Cmd(TIM3, ENABLE);    // TIM3 enable counter

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    NVIC_InitTypeDef nis;
    nis.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
    nis.NVIC_IRQChannelPriority = 2;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    DMA_InitTypeDef dis;
    DMA_DeInit(DMA1_Channel3);
    DMA_StructInit(&dis);
//    dis.DMA_BufferSize = 1;
    dis.DMA_PeripheralBaseAddr = (unsigned long)&(TIM3->CCR1);
    dis.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dis.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dis.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dis.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dis.DMA_Mode = DMA_Mode_Circular;
    dis.DMA_Priority = DMA_Priority_VeryHigh;
    dis.DMA_M2M = DMA_M2M_Disable;
    dis.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
    DMA_ITConfig(DMA1_Channel3, DMA_IT_HT, ENABLE);
    DMA_Init(DMA1_Channel3, &dis);
    printf("[%s: %d] TIMER Initialized.\n", __FILE__, __LINE__);
}
