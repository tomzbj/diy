#include "platform.h"

ZTASK zt(20, millis);

static void task_uart(void)
{
  if(Serial.available()) {
    char buf[128] = {0};
    int len = Serial.readBytes(buf, 126);
    zcli.parse(buf, len);
  }
}

static void task_led(void)
{
  constexpr int BLINK_GPIO = 7;    // 根据您的 ESP8684 / C2 开发板实际引脚修改
  static int state = 0;
  state = !state;
  digitalWrite(BLINK_GPIO, state);
}

static void task_hello(void)
{
  if(NET_WifiConnected())
    printf("Hello, world. %s udp:%u\n", WiFi.localIP().toString().c_str(),
      udp.local_port());
  else
    printf("Hello, world. wifi down\n");
}

void TASKS_Config(void)
{
  zt.bind(task_uart, 1, true);
  zt.bind(task_hello, 250, false);
  zt.bind(task_led, 50, true);
//  zt.bind([]() {disp.update(1);}, 1000, true);

  print_log();
}
