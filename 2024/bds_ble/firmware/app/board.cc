#include "misc.h"
#include "platform.h"

static uint8_t rxbuf[128], txbuf[128];
static uint8_t rxbuf2[1024], txbuf2[128];

const ZUart2::cb_t cbs = {    // USART1用于串口调试
    .readbyte_f = USART1_ReadByte, .writebyte_f = USART1_WriteByte};
const ZUart2::cb_t cbs2 = {    // USART2用于接收GPS/BDS模块数据
    .readbyte_f = USART2_ReadByte, .writebyte_f = USART2_WriteByte};
ZUart2 u1(cbs, rxbuf, sizeof(rxbuf), txbuf, sizeof(txbuf));
ZUart2 u2(cbs2, rxbuf2, sizeof(rxbuf2), txbuf2, sizeof(txbuf2));
ZCli cli(20);
CADC adc;
BDS bds;
CRTC rtc;

void BSP_Init(void)
{
    SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
    USART_Config();
    printf("[%s: %d] USART Initialized.\n", __FILE__, __LINE__);
    __HAL_RCC_PWR_CLK_ENABLE();
    u2.set_timeout(300);
    CLI_Config();
    bds.init();
    RF_Config();
    SHT_Config();
    adc.init();
    rtc.init();
    printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());
    printf("All peripherals initialized.\n");
}
