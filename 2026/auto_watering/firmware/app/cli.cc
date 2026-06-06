#include "misc.h"
#include "platform.h"

#define ok() printf("ok\n");

ZCli cli(16);

static void cli_wr_clear(const char* tokens[])
{
  uint16_t c = strtoul(tokens[2], nullptr, 16);
  extern class TFT_GC9307 tft;
  auto t1 = DWT->CYCCNT;
  tft.clear(c);
  auto t2 = DWT->CYCCNT;
  printf("%lu\n", t2 - t1);
  ok();
}

static void cli_wr_pwm(const char* tokens[])
{
  auto pwm = atoi(tokens[2]);
  TIM2->CCR1 = pwm;
  ok();
}

static void cli_wr_eeprom(const char* tokens[])
{
  int seed = atoi(tokens[2]);
  uint8_t data[16];
  int n = 1;
  for(int i = 0; i < 16; i++) {
    n *= seed;
    data[i] = n;
  }
  EEPROM_Write(data, 16);
  ok();
}

static void cli_rd_eeprom(void)
{
  uint8_t data[16] = {0};
  EEPROM_Read(data, 16);
  for(int i = 0; i < 16; i++) {
    printf("%02x ", data[i]);
  }
  printf("\n");
  ok();
}

static void cli_wr_motor(const char* tokens[])
{
  int state = atoi(tokens[2]);
  MOTOR_Set(state);
  ok();
}

static void cli_wr_sysclk(const char* tokens[])
{
  void set_clk_to_64m(void);
  void set_clk_to_24m(void);

  if(atoi(tokens[2]) == 64) {
    set_clk_to_64m();
    SystemCoreClock = 64000000UL;
  }
  else {
    set_clk_to_24m();
    SystemCoreClock = 24000000UL;
  }
  USART1_Config();
  ok();
}

static void cli_adc_get(void)
{
  constexpr int BUF_SIZE = 1024;
  uint16_t adc_data[BUF_SIZE] = {0};
  ADC_Get(adc_data, BUF_SIZE);

  for(int i = 0; i < BUF_SIZE; i++) {
    printf("%d\n", adc_data[i]);
  }
}

void CLI_Config(void)
{
  cli.bind("test", [] {puts("TEST OK");});
  cli.bind("reboot", [] {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd", [] {printf("%lu\n", HAL_RCC_GetSysClockFreq());});
  cli.bind("clear", "wr", cli_wr_clear);
  cli.bind("pwm", "wr", cli_wr_pwm);

  cli.bind("rtc", "rd", RTC_Show);
  cli.bind("rtc", "wr", [](const char* tokens[]) {RTC_SetTime(atoi(tokens[2]));ok();});

  cli.bind("page", "wr", [](const char*tokens[]) {
    extern class UI ui;
    ui.switch_page(static_cast<UI::PAGE>(atoi(tokens[2])));
  });

  cli.bind("eeprom", "wr", cli_wr_eeprom);
  cli.bind("eeprom", "rd", cli_rd_eeprom);
  cli.bind("motor", "wr", cli_wr_motor);
  cli.bind("sysclk", "wr", cli_wr_sysclk);
  cli.bind("adc", "rd", cli_adc_get);
  cli.bind("vbat", "rd", [] {printf("%d\n",ADC_GetVbat());});
  print_log();
}
