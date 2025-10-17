#include "misc.h"
#include "platform.h"

#define ok() printf("ok\n")

static void cli_rd_view_sinetab(const char* tokens[])
{
  DAC_ViewTab(printf);
  printf("### %lu\n", DAC_OUT0_R12DH(DAC0));
}

//static void cli_wr_adc_stop(const char** tokens) { dma_channel_disable(DMA1, DMA_CH0); ok(); }

static void cli_wr_adc_start(const char** tokens)
{
  const int n = 3200;
  static uint16_t buf[n] = {111, 222, 333};
  ADC_Start(buf, 1600);

  for(int i = 0; i < n; i++) {
    printf("%d\n", buf[i]);
    fflush(stdout);
  }
}

static void cli_rd_rf_poll(const char** tokens)
{
  extern class RF rf;
  uint16_t data[16384];
  rf.feed(data);
  rf.poll();
  ok();
}

static void cli_rd_params(const char** tokens)
{
  extern uint16_t tune;
  extern uint8_t bpf, vol;
  printf("%8d %8d %8d\n", tune,bpf,vol);
}

void CLI_Config(void)
{
  auto test_f = +[](const char**) {puts("TEST OK");};
  auto reboot_f = +[](const char**) {puts("Reboot..."); NVIC_SystemReset();};
  auto rd_freq_f = [](const char**) {printf("%lu\n", rcu_clock_freq_get(CK_SYS));};

  cli.bind("test", test_f);
  cli.bind("reboot", reboot_f);
  cli.bind("freq", "rd", rd_freq_f);
  cli.bind("tab", "rd", cli_rd_view_sinetab);
  cli.bind("adc_start", "wr", cli_wr_adc_start);
//  cli.bind("adc", "rd", cli_rd_adc);
  extern class RF rf;
  cli.bind("rf_poll", "rd", cli_rd_rf_poll);
  cli.bind("rf_stat", "rd", [](const char**) {rf.view_stat();});
  cli.bind("rf_config", "rd", [](const char**) {rf.view_config();});
  cli.bind("rf_nco", "rd", [](const char**) {rf.view_nco();});

  cli.bind("rf_volume", "wr", [](const char** tokens) {
    rf.set_volume( atoi(tokens[2]));
    ok();
  });
  cli.bind("rf_bpf", "wr", [](const char** tokens) {
    int bpf = saturate(atoi(tokens[2]), 0, 6);
    rf.set_bpf( bpf);
    ok();
  });

  cli.bind("rf_tune", "wr", [](const char** tokens) {
    rf.tune( atoi(tokens[2]));
    ok();
  });
  cli.bind("rf_view_rf", "rd", [](const char**) {rf.view_rf();});
  cli.bind("params", "rd", cli_rd_params);

  print_log();
}
