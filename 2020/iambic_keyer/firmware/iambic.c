#include "misc.h"
enum {
    dIDLE, dDASH, dDOT, dWAIT_DOT, dWAIT_DASH
};
enum {
    OFF, ON
};

#define dDOT_KEY GPIO_Pin_1
#define dDASH_KEY GPIO_Pin_0
#define WPM_LEVELS 4

static struct {
    bool dot_flag;
    int wpm_curr;
    int wpms[WPM_LEVELS];
} g = {false, 0, {12, 16, 20, 24}};

static void ToggleLED(void)
{
    GPIOA->BRR = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    switch(g.wpms[g.wpm_curr]) {
        case 12:
            GPIOA->BSRR = GPIO_Pin_0;
            break;
        case 16:
            GPIOA->BSRR = GPIO_Pin_1;
            break;
        case 20:
            GPIOA->BSRR = GPIO_Pin_2;
            break;
        case 24:
            GPIOA->BSRR = GPIO_Pin_3;
            break;
    }
}

void IAMBIC_ToggleWPM(void)
{
    g.wpm_curr++;
    if(g.wpm_curr >= (sizeof(g.wpms) / sizeof(g.wpms[0])))
        g.wpm_curr = 0;
    ToggleLED();
}

int IAMBIC_GetCurrWPM(void)
{
    return g.wpms[g.wpm_curr];
}

void IAMBIC_SetFlag(void)
{
    g.dot_flag = true;
}

void IAMBIC_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = dDASH_KEY | dDOT_KEY;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
            | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    ToggleLED();
}

void BEEP_Set(int state)
{
    if(state == ON) {
        TIM_Cmd(TIM14, ENABLE);
    }
    else {
        TIM_SetCounter(TIM14, 0);
        TIM_Cmd(TIM14, DISABLE);
    }
}

void IAMBIC_Loop(void)
{
    int vThisState = dIDLE, vNextState = dIDLE;
    int vCount = 0;
    while(1) {
        if(g.dot_flag) {
            g.dot_flag = false;
            vThisState = vNextState;
            switch(vThisState) {
                case dIDLE:
                    vNextState = dWAIT_DOT;
                    break;
                case dDOT:
                    BEEP_Set(ON);
                    vNextState = dWAIT_DOT;
                    break;
                case dDASH:
                    BEEP_Set(ON);
                    vCount++;
                    if(vCount >= 3) {
                        vNextState = dWAIT_DASH;
                        vCount = 0;
                    }
                    else
                        vNextState = dDASH;
                    break;
                case dWAIT_DOT:
                    BEEP_Set(OFF);
                    break;
                case dWAIT_DASH:
                    BEEP_Set(OFF);
                    break;
            } // end SWITCH
        } // end IF
          // ================================= Poll Key Inputs: Idle or a Wait State
        switch(vThisState) {
            case dIDLE:
                break;
            case dWAIT_DOT:
                if(!(GPIOF->IDR & dDASH_KEY))
                    vNextState = dDASH;  // Poll Key Inputs: DASH priority
                else if(!(GPIOF->IDR & dDOT_KEY))
                    vNextState = dDOT;
                else
                    vNextState = dIDLE;
                break;
            case dWAIT_DASH:
                if(!(GPIOF->IDR & dDOT_KEY))
                    vNextState = dDOT;   // Poll Key Inputs: DOT priority
                else if(!(GPIOF->IDR & dDASH_KEY))
                    vNextState = dDASH;
                else
                    vNextState = dIDLE;
                break;
            default:
                break;
        } // end SWITCH
    } // end WHILE
//#endif
} // (c)2018 - PP5VX ( V2.0b01 - 07 Jan 2018 )
