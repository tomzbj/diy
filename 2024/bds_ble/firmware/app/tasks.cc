#include "misc.h"
#include "platform.h"

/* @formatter:off*/
void task_rf(void)
{
    task_open();
    while(1) {
        RF_Poll();
        task_wait(500);     // 每500ms发送一次蓝牙广播
    }
    task_close();
}

void task_sht(void)
{
    task_open();
    while(1) {
//        SHT_Poll();
        sensor.poll();
        task_wait(1000);    // 每1000ms读取一次温湿度
    }
    task_close();
}

void task_led(void)
{
    task_open();
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef gis;
        gis.Pin = GPIO_PIN_5;
        gis.Mode = GPIO_MODE_OUTPUT_PP;
        gis.Pull = GPIO_PULLUP;
        gis.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &gis);
    }
    while(1) {
        if(bds.sync == false)  {
            ZPin::toggle(PA5);
        }
        else {
            static int count = 0;
            ++count %= 10;
            ZPin::write(PA5, (count == 0));
        }
        task_wait(100);     // 未成功对时则50%占空比闪烁, 成功则10%占空比
    }
    task_close();
}

void task_uart(void)
{
    task_open();
    while(1) {
        if(u1.idle()) {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            char c = u1.peek();
            int len = u1.read(buf, sizeof(buf));
            if(c == '#')
                cli.parse(buf, len);
        }
        if(u2.idle()) {
            char buf[1024];
            int len = u2.read(buf, sizeof(buf));
            bds.parse(buf, len);
        }
        task_wait(10);      // 读取串口调试和GPS/BDS模块数据
    }
    task_close();
}

void task_bds(void)
{
    task_open();
    while(1) {
        bds.sync = false;
        bds.on();
        task_wait(1000UL * 3600UL * 17);    // 每17小时重新打开GPS/BDS模块
    }
    task_close();
}

/* @formatter:on*/

void TASKS_Config(void)
{
    os_init();

    task_create(task_uart, NULL, 1, NULL, 0, 0);
    task_create(task_led, NULL, 4, NULL, 0, 0);
    task_create(task_rf, NULL, 5, NULL, 0, 0);
    task_create(task_sht, NULL, 6, NULL, 0, 0);
    task_create(task_bds, NULL, 3, NULL, 0, 0);

    os_start();
}
