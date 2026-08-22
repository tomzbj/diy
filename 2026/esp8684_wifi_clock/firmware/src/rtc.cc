#include "platform.h"
#include <Preferences.h>
#include <sys/time.h>
#include <time.h>

static const char* NVS_NS = "app";

static uint32_t nvs_get_u32(const char* key, uint32_t def)
{
  Preferences nvs;
  nvs.begin(NVS_NS, false);
  uint32_t v = nvs.getULong(key, def);
  nvs.end();
  return v;
}

static void nvs_put_u32(const char* key, uint32_t v)
{
  Preferences nvs;
  nvs.begin(NVS_NS, false);
  nvs.putULong(key, v);
  nvs.end();
}

static bool time_valid(time_t now)
{
  return now >= (time_t)TIME_VALID_EPOCH;
}

static bool in_ntp_window(time_t now)
{
  struct tm tm;
  localtime_r(&now, &tm);
  return tm.tm_hour >= NTP_HOUR_START && tm.tm_hour < NTP_HOUR_END;
}

int RTC_GetBootCount(void)
{
  return 0;    //(int)nvs_get_u32("boot_cnt", 0);
}

void RTC_Config(void)
{
  setenv("TZ", NTP_TZ, 1);
  tzset();

  /*  uint32_t boot_cnt = nvs_get_u32("boot_cnt", 0) + 1;
   nvs_put_u32("boot_cnt", boot_cnt);
   printf("boot_cnt %lu last_sync %lu\n", (unsigned long)boot_cnt,
   (unsigned long)nvs_get_u32("last_sync", 0));*/
}

void RTC_MaybeNtp(void)
{
  time_t now = time(NULL);
  uint32_t last_sync = nvs_get_u32("last_sync", 0);
  bool need = (last_sync == 0) || !time_valid(now)
    || (time_valid(now) && (now - (time_t)last_sync) >= NTP_SYNC_INTERVAL_S
      && in_ntp_window(now));

  if(!need) {
    printf("[NTP] skip, now %ld last_sync %lu\n", (long)now, (unsigned long)last_sync);
    return;
  }

  printf("[NTP] syncing...\n");
  if(!NET_WifiUp())
    return;
  if(NET_NtpSync(NTP_TIMEOUT_MS))
    nvs_put_u32("last_sync", (uint32_t)time(NULL));
  NET_WifiDown();
}

void RTC_DeepSleep(uint64_t time_us)
{
  NET_WifiDown();
  disp.hold_io_for_sleep();
  printf("deep sleep %d s\n", (int)(time_us / 1000000ULL));
  Serial.flush();
  ESP.deepSleep(time_us);
}
