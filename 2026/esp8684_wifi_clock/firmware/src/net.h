#ifndef _NET_H
#define _NET_H

#include <cstddef>
#include <cstdint>

void NET_Config(void);
bool NET_WifiConnected(void);
bool NET_WifiUp(void);
void NET_WifiDown(void);
bool NET_NtpSync(uint32_t timeout_ms);

bool NET_HasCreds(void);
bool NET_CredsLoad(char* ssid, size_t ssid_len, char* pass, size_t pass_len);
void NET_CredsSave(const char* ssid, const char* pass);
void NET_CredsClear(void);
void NET_MakeApId(char* ssid, size_t ssid_len, char* pass, size_t pass_len);
void NET_RunProvision(const char* ap_ssid, const char* ap_pass);

#endif
