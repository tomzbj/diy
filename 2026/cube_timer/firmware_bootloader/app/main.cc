#include "misc.h"
#include "zboot.h"
#include "flash.h"
#include "xprintf.h"
#include "stm32f1xx_hal_conf.h"
#include "zuart4.h"
#include "usart1_hal.h"

const ZUART4::CBS cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};

const ZBOOT::CBS cbs_zboot = {.printf_f = (int (*)(const char*, ...))xprintf,    //
  .usart_write_f = USART1_Write, .crc32_f = nullptr, .reboot_f = NVIC_SystemReset,    //
  .page_erase_f = flash_page_erase, .flash_program_f = flash_program};

ZUART4 u1(cbs_u1, 65536);
ZBOOT zboot(cbs_zboot, 0x0800'0000, 1024 * 1024, 2048, 60000);

constexpr int jump_delay = 300;    // wait 2000ms and then jump to app
static uint8_t packet_buf[61000];
static bool flag_jump = true;

int main(void)
{
  *(unsigned long*)0xe000ed24 = 0x00070000; // enable usage fault
  SystemCoreClock = HAL_RCC_GetSysClockFreq();
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  u1.set_timeout(100);
  USART1_Config();
  xprintf("Boot done.\n");

  while(1) {
    if(millis() > jump_delay && flag_jump == true && zboot.app_is_valid())
      zboot.jump_to_app();
    if(u1.available() > 0) {
      flag_jump = false;

      char c = u1.peek();
      if(c == '#' && u1.idle()) {
        char buf[128] = {0};
        int len = u1.read(buf, sizeof(buf));
        zboot.parse_cmd(buf, len);
      }
      else if(c == zboot.ID) {    // binary header
        if(u1.available() >= 4) {
          uint32_t header = 0;
          u1.peek(&header, 4);
          uint32_t packet_size = header >> 16;

          if(u1.idle()) {    // timeout
            u1.clear();
            continue;
          }
          else if(u1.available() < packet_size)    // wait for more data
            continue;

          u1.read(packet_buf, packet_size);
          zboot.parse_binary(packet_buf, packet_size);
        }
      }
      else if(u1.idle()) {
        u1.clear();
      }
    }
  }
}
