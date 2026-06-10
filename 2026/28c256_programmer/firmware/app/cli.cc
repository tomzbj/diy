#include "misc.h"
#include "platform.h"
#include <cstdarg>

auto ok = +[]() {printf("ok\n");};

ZCli cli(20);
ZCli cli_cdc(20);

static int usb_printf(const char* fmt, ...)
{
  char buf[256] = {0};
  va_list arp;
  va_start(arp, fmt);
  int n = vsnprintf(buf, sizeof(buf), fmt, arp);
  USB_Write((uint8_t*)buf, n);
  va_end(arp);
  return n;
}

constexpr int BUF_SIZE = 128;

static void write_eeprom(const char* tokens[])
{
  uint8_t buf[BUF_SIZE] = {0};
  int n = atoi(tokens[2]);
  int k = 1;
  for(int i = 0; i < BUF_SIZE; i++) {
    k *= n;
    buf[i] = k;
  }
  at28cxx.write(0, buf, BUF_SIZE);
  ok();
}

static void read_eeprom(void)
{
  uint8_t buf[BUF_SIZE] = {0};
  at28cxx.read(0, buf, BUF_SIZE);
  for(int i = 0; i < BUF_SIZE; i++) {
    printf("%02x ", buf[i]);
    if(i % 16 == 15)
      printf("\n");
  }
  printf("\n");
}

void CLI_Config(void)
{
  cli.bind("test", [] {printf("TEST OK\n");});
  cli.bind("reboot", [] {printf("REBOOT.\n"); NVIC_SystemReset();});
  cli.bind("freq", "rd", [] {printf("%lu\n", HAL_RCC_GetSysClockFreq());});

  cli.bind("eeprom", "wr", write_eeprom);
  cli.bind("eeprom", "rd", read_eeprom);

  cli_cdc.bind("test", [] {usb_printf("TEST OK\n");});
  cli_cdc.bind("reboot", [] {usb_printf("REBOOT.\n"); NVIC_SystemReset();});
  cli_cdc.bind("freq", "rd", [] {usb_printf("%lu\n", HAL_RCC_GetSysClockFreq());});

  print_log();
}
