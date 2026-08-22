#include "platform.h"

DISP disp;

void setup(void)
{
  pinMode(7, OUTPUT);    // for led
  pinMode(KEY_PIN, INPUT_PULLUP);
  delay(5);
  bool provision = (digitalRead(KEY_PIN) == LOW);

  Serial.begin(1500000UL);
  Serial.setTimeout(3);
  printf("\n\n");
  if(provision)
    printf("[WiFi] KEY held, provisioning\n");

  TASKS_Config();
  SHT_Config();
  CLI_Config();
  RTC_Config();
  disp.init();
  delay(80);

  if(provision) {
    char ap_ssid[20], ap_pass[12];
    NET_MakeApId(ap_ssid, sizeof(ap_ssid), ap_pass, sizeof(ap_pass));
    disp.show_provision(ap_ssid, ap_pass);
    NET_RunProvision(ap_ssid, ap_pass);
    return;
  }

  if(!NET_HasCreds()) {
    printf("[WiFi] no STA credentials\n");
    disp.show_need_setup();
    delay(500);
    RTC_DeepSleep(SLEEP_US);
    return;
  }

  RTC_MaybeNtp();
  sensor.poll();
  disp.update(1);
  delay(300);

  printf("All peripherals initialized.\n");
  printf("Chip Model: %s SYSCLK: %dMHz\n\n", ESP.getChipModel(), getCpuFrequencyMhz());
  RTC_DeepSleep(SLEEP_US);
}

void loop(void)
{
  zt.poll();
}
