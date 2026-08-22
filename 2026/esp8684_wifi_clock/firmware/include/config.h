#pragma once

#include "credentials.h"

#define WIFI_TIMEOUT_S  20
#define WIFI_RETRY_MS   10000
#define WIFI_SSID_MAX   32
#define WIFI_PASS_MAX   63
#define KEY_PIN         6
#define AP_SSID_PREFIX  "C8E"

#define UDP_PORT        5000

#define NTP_TZ              "CST-8"
#define NTP_SERVER1         "ntp.aliyun.com"
#define NTP_SERVER2         "ntp.ntsc.ac.cn"
#define NTP_SERVER3         "pool.ntp.org"
#define NTP_TIMEOUT_MS      15000
#define NTP_SYNC_INTERVAL_S (24L * 3600)
#define NTP_HOUR_START      2
#define NTP_HOUR_END        5
#define TIME_VALID_EPOCH    1577836800UL
#define SLEEP_US            (60ULL * 1000000ULL)
