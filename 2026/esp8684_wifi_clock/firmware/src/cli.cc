#include "platform.h"
#include <WiFi.h>

#define ok() printf("ok\n");

ZCli zcli(30);

static void cli_rd_ram(void)
{
  printf("%lu %lu %lu\n",    //
    ESP.getFreeHeap(), ESP.getMinFreeHeap(), ESP.getMaxAllocHeap());
}

static void cli_wr_freq(const char* tokens[])
{
  int freq = atoi(tokens[2]);
  setCpuFrequencyMhz(freq);
  printf("%lu MHz\n", getCpuFrequencyMhz());
}

static void cli_rd_ip(void)
{
  if(!NET_WifiConnected()) {
    puts("wifi down");
    return;
  }
  printf("%s gw %s rssi %d udp %u\n", WiFi.localIP().toString().c_str(),
    WiFi.gatewayIP().toString().c_str(), WiFi.RSSI(), udp.local_port());
}

void CLI_Config(void)
{
  zcli.bind("test", []() {puts("TEST OK");});
  zcli.bind("reboot", []() {puts("REBOOT."); ESP.restart();});
  zcli.bind("freq", "rd", []() {printf("%lu MHz\n", getCpuFrequencyMhz() );});

  zcli.bind("sleep", "wr", []() {RTC_DeepSleep(10000000UL);});

  zcli.bind("ram", "rd", cli_rd_ram);
  zcli.bind("freq", "wr", cli_wr_freq);
  zcli.bind("ip", "rd", cli_rd_ip);

  zcli.bind("sht", "rd", []() {sensor.poll(); printf("%d %d\n", sensor.RH, sensor.T);});
  zcli.bind("disp_update", "wr", []() {disp.update(1);});
  zcli.bind("boot_cnt", "rd", []() {printf("%d\n", RTC_GetBootCount());});
  zcli.bind("ssid", "rd", []() {
    char ssid[WIFI_SSID_MAX + 1];
    if(NET_CredsLoad(ssid, sizeof(ssid), nullptr, 0))
      printf("%s\n", ssid);
    else
      puts("(none)");
  });
  zcli.bind("wifi_erase", "wr", []() {NET_CredsClear(); puts("ok");});

  print_log();
}
