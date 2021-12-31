#include "platform.h"
#include "misc.h"

static int count;

#define set_tim6_freq(freq)    TIM_SetAutoreload(TIM6, (int)(72000000UL / 256.0 / freq - 1))
static zi2c_t mcp_echo, mcp_output;

const int volume[] = {0, 1, 3, 5, 8, 15, 25, 43, 74, 127};

void RF_SetEchoVolume(int vol)
{
    if(vol < 0)
        vol = 0;
    if(vol > 9)
        vol = 9;
    MCP4018_Write(&mcp_echo, volume[vol]);
}

void RF_SetOutputVolume(int vol)
{
    if(vol < 0)
        vol = 0;
    if(vol > 9)
        vol = 9;
    MCP4018_Write(&mcp_output, volume[vol]);
}

void GUI_AddChar(char c);
void GUI_ClearMultiEdit(void);

static void set_dac_trig_source_t6_trgo(int s)
{
    unsigned long tmp = (DAC->CR & ~0x3CUL);
    if(s)
        tmp |= DAC_Trigger_T6_TRGO;
    else
        tmp |= DAC_Trigger_None;
    DAC->CR = tmp;
}

static void cw_tx_start(void)
{
    DAC_DMACmd(DAC1, DAC_Channel_1, ENABLE);
    GPIOB->BRR = GPIO_Pin_11;
    set_dac_trig_source_t6_trgo(1);
    DataConfig_t* pdc = DC_Get();
    set_tim6_freq(pdc->cfg.cw_carrier_freq);
    GUI_ClearMultiEdit();
}

static void bpsk_tx_start(void)
{
    GPIOB->BRR = GPIO_Pin_11;
    set_dac_trig_source_t6_trgo(0);
    GUI_ClearMultiEdit();
}

static void bpsk_tx_char(int c)
{
    printf("%c", c);
    fflush(stdout);
    GUI_AddChar(c);
}

static void bpsk_tx_end(void)
{
    DAC_SoftwareTriggerCmd(DAC1, DAC_Channel_1, DISABLE);
    GPIOB->BSRR = GPIO_Pin_11;
    printf("bpsk tx end.\n");
}

static void bpsk_tx_symbol(unsigned short data)
{
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.tx_mode == DC_TX_MODE_BPSK) {
        DAC1->DHR12L1 = data >> 1;
    }
}

static void cw_tx_char(int c)
{
    printf("%c", c);
    fflush(stdout);
    GUI_AddChar(c);
}

static void cw_tx_end(void)
{
    printf("cw tx finished!\n");
    GPIOB->BSRR = GPIO_Pin_11;
}

static void cw_tx_symbol(int c)
{
    static int oldc = 1;

    static int c1, c0;
    if(oldc != c) {
        c0 = 0;
        c1 = 0;
    }
    if(c == 0)
        c0++;
    else
        c1++;
//    fflush(stdout);
    oldc = c;
    if(c)
        TIM_Cmd(TIM6, ENABLE);
    else
        TIM_Cmd(TIM6, DISABLE);
}

static void ft8_tx_start(void)
{
    DAC_DMACmd(DAC1, DAC_Channel_1, ENABLE);
    GPIOB->BRR = GPIO_Pin_11;
    set_dac_trig_source_t6_trgo(1);
    count = 0;
    TIM_Cmd(TIM6, ENABLE);
    GUI_ClearMultiEdit();
}

static void ft8_tx_end(void)
{
    printf("\n");
    TIM_Cmd(TIM6, DISABLE);
    GPIOB->BSRR = GPIO_Pin_11;
}

static void ft8_tx_symbol(int c)
{
    printf("%d:%d\n", count, c);
    count++;
    DataConfig_t* pdc = DC_Get();
    float freq = pdc->cfg.ft8_carrier_freq + c * 6.25;
    set_tim6_freq(freq);
}

static void setsda(int state)
{
    (state) ? (GPIOB->BSRR = GPIO_Pin_0) : (GPIOB->BRR = GPIO_Pin_0);
    _delay_us(30);
}

static void setscl(int state)
{
    (state) ? (GPIOB->BSRR = GPIO_Pin_1) : (GPIOB->BRR = GPIO_Pin_1);
    _delay_us(30);
}

static int getsda(void)
{
    _delay_us(30);
    return (GPIOB->IDR & GPIO_Pin_0);
}
static void setsda2(int state)
{
    (state) ? (GPIOB->BSRR = GPIO_Pin_2) : (GPIOB->BRR = GPIO_Pin_2);
    _delay_us(30);
}

static void setscl2(int state)
{
    (state) ? (GPIOB->BSRR = GPIO_Pin_10) : (GPIOB->BRR = GPIO_Pin_10);
    _delay_us(30);
}

static int getsda2(void)
{
    _delay_us(30);
    return (GPIOB->IDR & GPIO_Pin_2);
}

void RF_Config(void)
{
    static cw_cbs_t cw_cbs;
    static bpsk31_cbs_t bpsk_cbs;
    static ft8_cbs_t ft8_cbs;

    cw_cbs.tx_char_f = cw_tx_char;
    cw_cbs.tx_symbol_f = cw_tx_symbol;
    cw_cbs.tx_start_f = cw_tx_start;
    cw_cbs.tx_end_f = cw_tx_end;
    cw_init(&cw_cbs);
    bpsk_cbs.tx_char_f = bpsk_tx_char;
    bpsk_cbs.tx_start_f = bpsk_tx_start;
    bpsk_cbs.tx_end_f = bpsk_tx_end;
    bpsk_cbs.tx_symbol_f = bpsk_tx_symbol;
    bpsk31_init(&bpsk_cbs);
    ft8_cbs.tx_start_f = ft8_tx_start;
    ft8_cbs.tx_end_f = ft8_tx_end;
    ft8_cbs.tx_symbol_f = ft8_tx_symbol;
    ft8_init(&ft8_cbs);

    mcp_echo.setsda_f = setsda;
    mcp_echo.setscl_f = setscl;
    mcp_echo.getsda_f = getsda;
    mcp_echo.addr = 0x2f;
    mcp_echo.retry_count = 50;
    zi2c_init(&mcp_echo);
    mcp_output.setsda_f = setsda2;
    mcp_output.setscl_f = setscl2;
    mcp_output.getsda_f = getsda2;
    mcp_output.addr = 0x2f;
    mcp_output.retry_count = 50;
    zi2c_init(&mcp_output);

//    MCP4018_Write(&mcp_echo, 0x7f);
//    MCP4018_Write(&mcp_output, 0x7f);
    DataConfig_t* pdc = DC_Get();
    RF_SetEchoVolume(pdc->cfg.echo_volume);
    RF_SetOutputVolume(pdc->cfg.output_volume);

    printf("[%s: %d] RF Initialized.\n", __FILE__, __LINE__);
}

void RF_Parse(const void* msg, int size)
{
    const char* pmsg = msg;
//printf("### %d %s ###\n", size, msg);
    DataConfig_t* pdc = DC_Get();
    if(pdc->cfg.tx_mode == DC_TX_MODE_CW)
        cw_tx(pmsg, size);
    else if(pdc->cfg.tx_mode == DC_TX_MODE_BPSK)
        bpsk31_tx((const unsigned char*)pmsg, size);
    else if(pdc->cfg.tx_mode == DC_TX_MODE_FT8)
        ft8_tx(pmsg, size);
}
