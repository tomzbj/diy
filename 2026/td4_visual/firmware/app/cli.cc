#include "misc.h"
#include "platform.h"

ZCli cli(20);

#define ok() printf("ok\n");

static void cli_wr_draw_rect(const char* tokens[])
{
  uint16_t buf[1024];
  uint16_t c = strtoul(tokens[2], nullptr, 16);
  for(int i = 0; i < 1024; i++)
    buf[i] = c;
  int x1 = atoi(tokens[3]);
  int y1 = atoi(tokens[4]);
  int x2 = atoi(tokens[5]);
  int y2 = atoi(tokens[6]);
  tft.set_window(x1, y1, x2, y2);
  if(x2 < x1)
    std::swap(x1, x2);
  if(y2 < y1)
    std::swap(y1, y2);
  int count = (x2 - x1 + 1) * (y2 - y1 + 1);
  if(count > 1024)
    count = 1024;
  tft.write(buf, count);
  ok();
}

static void cli_wr_clear(const char* tokens[])
{
  uint16_t c = strtoul(tokens[2], nullptr, 16);
  auto t1 = millis();
  tft.clear(c);
  auto t2 = millis();
  printf("%lu\n", t2 - t1);
  ok();
}

static void cli_rd_rom(void)
{
  uint8_t ins[16];
  rom.read_instructions(ins, 16);

  for(int i = 0; i < 16; i++) {
    printf("%02X ", ins[i]);
  }
  printf("\n");
}

void CLI_Config(void)
{
  cli.bind("test", []() {puts("TEST OK");});
  cli.bind("reboot", []() {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd", []() {printf("%lu\n", HAL_RCC_GetSysClockFreq());});

  // cpu
//  cli.bind("addr", "rd", []() {printf("%1X\n", GPIOB->IDR >> 12);});
  cli.bind("rom", "rd", cli_rd_rom);
  // gui
  cli.bind("clear", "wr", cli_wr_clear);
  cli.bind("draw_rect", "wr", cli_wr_draw_rect);
  print_log();
}
