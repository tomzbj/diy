#include "platform.h"

void BDS::parse(const void* msg, int size)
{
    auto p = (const char*)msg;
    int t = 0;
    sscanf(strstr(p, "$GNGGA") + 7, "%d", &t);
    if(t == 0)
        sscanf(strstr(p, "$GNRMC") + 7, "%d", &t);
    if(t == 0)
        sscanf(strstr(p, "$GNZDA") + 7, "%d", &t);
    if(t != 0) {
        rtc.set_time(t / 10000, (t / 100) % 100, t % 100);
        printf("############## sync ok ################\n");
        sync = true;
        off();  // 对时成功则关闭GPS/BDS模块电源
    }
}

void BDS::init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gis;
    gis.Pin = GPIO_PIN_0;
    gis.Mode = GPIO_MODE_OUTPUT_PP;
    gis.Pull = GPIO_PULLUP;
    gis.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gis);
    on();
    sync = false;
}
