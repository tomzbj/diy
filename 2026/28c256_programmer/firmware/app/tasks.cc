#include "misc.h"
#include "platform.h"
#include "usb_config.h"

ZTASK zt(20, millis);

static uint8_t packet_buf[2020] = {0};

static void task_uart(void)
{
  if(u1.available() > 0) {
    char c = u1.peek();
    if(c == '#' && u1.idle()) {
      char buf[128] = {0};
      int len = u1.read(buf, sizeof(buf));
      cli.parse(buf, len);
    }
    else if(c == flasher.ID) {    // binary header
      if(u1.available() >= 4) {
        uint32_t header = 0;
        u1.peek(&header, 4);
        uint32_t packet_size = header >> 16;
        if(u1.idle()) {    // timeout
          u1.clear();
          return;
        }
        else if(u1.available() < packet_size)    // wait for more data
          return;
        u1.read(packet_buf, packet_size);
        flasher.parse_binary(packet_buf, packet_size);
      }
    }
    else if(u1.idle()) {
      u1.clear();
    }
  }

  if(ucdc.available() > 0) {
    char c = ucdc.peek();
    if(c == '#' && ucdc.idle()) {
      char buf[128] = {0};
      int len = ucdc.read(buf, sizeof(buf));
      cli_cdc.parse(buf, len);
    }
    else if(c == flasher.ID) {    // binary header
      if(ucdc.available() >= 4) {
        uint32_t header = 0;
        ucdc.peek(&header, 4);
        uint32_t packet_size = header >> 16;
        if(ucdc.idle()) {    // timeout
          ucdc.clear();
          return;
        }
        else if(ucdc.available() < packet_size)    // wait for more data
          return;
        ucdc.read(packet_buf, packet_size);
        flasher_u.parse_binary(packet_buf, packet_size);
      }
    }
    else if(ucdc.idle()) {
      ucdc.clear();
    }
  }

}

void TASKS_Config(void)
{
  if(1) {
    GPIO_InitTypeDef gis;
    __HAL_RCC_GPIOF_CLK_ENABLE();
    gis.Pin = GPIO_PIN_5;
    gis.Mode = GPIO_MODE_OUTPUT_PP;
    gis.Pull = GPIO_PULLUP;
    gis.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOF, &gis);
  }
//  zt.bind([] {printf("Hello, world.\n");}, 500, true);
  zt.bind(task_uart, 1, true);
  zt.bind([] {ZPin::toggle(PF5);}, 50, true);
  print_log();
}
