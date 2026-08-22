#include "platform.h"
#include "config.h"
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include "esp_mac.h"

UDP_SERVER udp;

static const char* NVS_NS = "app";

static bool wifi_connect(void)
{
  char ssid[WIFI_SSID_MAX + 1];
  char pass[WIFI_PASS_MAX + 1];
  if(!NET_CredsLoad(ssid, sizeof(ssid), pass, sizeof(pass))) {
    printf("[WiFi] no credentials\n");
    return false;
  }

  printf("[WiFi] Connecting to %s\n", ssid);
  Serial.flush();
  disableLoopWDT();
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  enableLoopWDT();

  unsigned long start = millis();
  while(WiFi.status() != WL_CONNECTED) {
    if(millis() - start > WIFI_TIMEOUT_S * 1000UL) {
      printf("[WiFi] Connection timed out\n");
      WiFi.disconnect();
      return false;
    }
    delay(100);
  }

  printf("[WiFi] Connected, IP: %s\n", WiFi.localIP().toString().c_str());
  return true;
}

bool NET_WifiConnected(void)
{
  return WiFi.status() == WL_CONNECTED;
}

bool NET_WifiUp(void)
{
  if(NET_WifiConnected())
    return true;
  return wifi_connect();
}

void NET_WifiDown(void)
{
  if(WiFi.getMode() == WIFI_MODE_NULL)
    return;
  WiFi.disconnect(true, false);
  WiFi.mode(WIFI_OFF);
}

bool NET_NtpSync(uint32_t timeout_ms)
{
  configTzTime(NTP_TZ, NTP_SERVER3, NTP_SERVER2, NTP_SERVER1);
  struct tm info;
  if(!getLocalTime(&info, timeout_ms)) {
    printf("[NTP] sync timed out\n");
    return false;
  }
  printf("[NTP] %04d-%02d-%02d %02d:%02d:%02d\n", info.tm_year + 1900,
    info.tm_mon + 1, info.tm_mday, info.tm_hour, info.tm_min, info.tm_sec);
  return true;
}

void NET_Config(void)
{
  while(!wifi_connect()) {
    printf("[WiFi] Retrying in %ds...\n", WIFI_RETRY_MS / 1000);
    delay(WIFI_RETRY_MS);
  }

  if(udp.begin(UDP_PORT))
    printf("[UDP] Listening on %s:%d\n", WiFi.localIP().toString().c_str(), UDP_PORT);
  else
    printf("[UDP] begin(%d) failed\n", UDP_PORT);

  print_log();
}

bool NET_CredsLoad(char* ssid, size_t ssid_len, char* pass, size_t pass_len)
{
  if(!ssid || ssid_len < 2)
    return false;
  ssid[0] = 0;
  if(pass && pass_len)
    pass[0] = 0;

  Preferences nvs;
  if(!nvs.begin(NVS_NS, true))
    return false;
  if(!nvs.isKey("ssid")) {
    nvs.end();
    return false;
  }
  size_t n = nvs.getString("ssid", ssid, ssid_len);
  if(pass && pass_len && nvs.isKey("pass"))
    nvs.getString("pass", pass, pass_len);
  nvs.end();
  return n > 0 && ssid[0] != 0;
}

bool NET_HasCreds(void)
{
  char ssid[WIFI_SSID_MAX + 1];
  return NET_CredsLoad(ssid, sizeof(ssid), nullptr, 0);
}

void NET_CredsSave(const char* ssid, const char* pass)
{
  Preferences nvs;
  nvs.begin(NVS_NS, false);
  nvs.putString("ssid", ssid ? ssid : "");
  nvs.putString("pass", pass ? pass : "");
  nvs.end();
}

void NET_CredsClear(void)
{
  Preferences nvs;
  nvs.begin(NVS_NS, false);
  nvs.remove("ssid");
  nvs.remove("pass");
  nvs.end();
}

void NET_MakeApId(char* ssid, size_t ssid_len, char* pass, size_t pass_len)
{
  uint8_t mac[6] = {0};
  esp_read_mac(mac, ESP_MAC_WIFI_SOFTAP);
  snprintf(ssid, ssid_len, "%s-%02X%02X%02X", AP_SSID_PREFIX, mac[3], mac[4], mac[5]);
  snprintf(pass, pass_len, "%02X%02X%02X%02X", mac[2], mac[3], mac[4], mac[5]);
}

static const char PROV_PAGE[] =
  "<!DOCTYPE html><html><head><meta charset=utf-8>"
  "<meta name=viewport content='width=device-width,initial-scale=1'>"
  "<title>WiFi</title></head><body>"
  "<h3>WiFi setup</h3>"
  "<form method=POST action=/save>"
  "SSID<br><input name=ssid maxlength=32 required><br><br>"
  "Password<br><input name=pass type=password maxlength=63><br><br>"
  "<input type=submit value=Save>"
  "</form></body></html>";

void NET_RunProvision(const char* ap_ssid, const char* ap_pass)
{
  WebServer portal(80);
  DNSServer dns;

  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  delay(50);
  if(!WiFi.softAP(ap_ssid, ap_pass)) {
    printf("[WiFi] softAP failed\n");
    return;
  }

  IPAddress ip = WiFi.softAPIP();
  printf("[WiFi] AP %s  pass %s  http://%s/\n", ap_ssid, ap_pass, ip.toString().c_str());
  dns.start(53, "*", ip);

  portal.on("/", HTTP_GET, [&]() { portal.send(200, "text/html", PROV_PAGE); });
  portal.on("/save", HTTP_POST, [&]() {
    String ssid = portal.arg("ssid");
    String pass = portal.arg("pass");
    ssid.trim();
    if(ssid.length() < 1 || ssid.length() > WIFI_SSID_MAX) {
      portal.send(400, "text/plain", "bad ssid");
      return;
    }
    if(pass.length() > WIFI_PASS_MAX)
      pass.remove(WIFI_PASS_MAX);
    NET_CredsSave(ssid.c_str(), pass.c_str());
    printf("[WiFi] saved ssid %s\n", ssid.c_str());
    portal.send(200, "text/html",
      "<!DOCTYPE html><html><body>Saved. Rebooting...</body></html>");
    portal.client().flush();
    delay(400);
    ESP.restart();
  });
  portal.onNotFound([&]() {
    portal.sendHeader("Location", "http://192.168.4.1/", true);
    portal.send(302, "text/plain", "");
  });
  portal.begin();

  for(;;) {
    dns.processNextRequest();
    portal.handleClient();
    zt.poll();
    delay(2);
  }
}
