#include "misc.h"
#include "platform.h"
#include <unordered_map>
#include <tuple>

#define ok() printf("ok\n");

static void cli_wr_dac(const char* tokens[])
{
  int dac = atoi(tokens[2]);
  DAC->DHR12R1 = dac;
  printf("%d\n", dac);
  ok();
}

static void cli_wr_freq(const char* tokens[])
{
  int freq = atoi(tokens[2]);
  int freq2 = atoi(tokens[3]);
  dds.set_freq(freq, freq2);
  printf("%d %d\n", freq, freq2);
  ok();
}

static void cli_wr_dtmf2(const char* tokens[])
{
  int len = strlen(tokens[2]);
  mod.tx(tokens[2], len);
}

/* static void cli_wr_dtmf(const char* tokens[]) {
  std::unordered_map<char, std::tuple<int, int> > tone = { //
    {'1', {697, 1209}}, {'2', {697, 1336}}, {'3', {697, 1477}}, {'4', {770, 1209}},    //
    {'5', {770, 1336}}, {'6', {770, 1477}}, {'7', {852, 1209}}, {'8', {852, 1336}},    //
    {'9', {852, 1477}}, {'A', {697, 1633}}, {'B', {770, 1633}}, {'C', {852, 1633}},    //
    {'D', {941, 1633}}, {'#', {941, 1477}}, {'0', {941, 1336}}, {'*', {941, 1209}},    //
    };
  int n = strlen(tokens[2]);
  if(n > 15)
    n = 15;
  for(int i = 0; i < n; i++) {
    auto c = tokens[2][i];
    auto [x, y] = tone[c];
    dds.set_freq(x, y);
    HAL_Delay(200);
    dds.set_freq(1, 1);
    HAL_Delay(20); } } */

void CLI_Config(void)
{
  cli.bind("test", [] { _dbg(); puts("TEST OK");});
  cli.bind("reboot", [] {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd", [] {printf("%lu\n", HAL_RCC_GetSysClockFreq());});

  cli.bind("t1cnt", "rd", [] {printf("%lu\n", TIM1->CNT);});
  cli.bind("dac", "wr", cli_wr_dac);
  cli.bind("freq", "wr", cli_wr_freq);
//  cli.bind("dtmf", "wr", cli_wr_dtmf);
  cli.bind("dtmf2", "wr", cli_wr_dtmf2);
  cli.bind("view_data", "rd", [] {dem.view_data();});
  cli.bind("view_power", "rd", [] {dem.view_power();});
  cli.bind("view_time", "rd", [] {dem.view_time();});

  print_log();
}
