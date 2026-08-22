#include "misc.h"
#include "platform.h"
#include "st7305.h"
#include "disp.h"
#include <SPI.h>
#include <time.h>
#include "driver/gpio.h"

#define ZP_MOSI  0
#define ZP_SCK   1
#define ZP_CS    2
#define ZP_RS    3
#define ZP_RESET 4
#define ZP_BUSY  5
#define ZP_SPI_HZ 10000000UL

auto lcd_delay_us = +[](volatile int nus) {delayMicroseconds(nus);};
auto setrs_f = +[](int s) {lcd_delay_us(1);digitalWrite(ZP_RS, s);lcd_delay_us(1);};
auto setcs_f = +[](int s) {lcd_delay_us(1);digitalWrite(ZP_CS, s);lcd_delay_us(1);};
auto reset_f = +[](int s) {lcd_delay_us(1);digitalWrite(ZP_RESET, s);lcd_delay_us(1);};

auto SPI_Write = [](const void* msg, int size) {    //
    SPI.writeBytes((const uint8_t*)msg, size);
  };
static ST7305::cbs_t cbs = {.reset_f = reset_f, .cs_f = setcs_f, .rs_f = setrs_f,
  .fastwrite_f = SPI_Write, .delay_us_f = lcd_delay_us};

ST7305 st7305(cbs);

static void write_vram(int x1, int y1, int x2, int y2, const void* data)
{
  uint32_t n = (y2 - y1 + 1) * (x2 - x1 + 1) / 8;
  st7305.write((uint8_t*)data, n);
}

void DISP::normal(void)
{
  char timestr[16] = {0};
  char date[32] = {0};
  const char* mon[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
    "Oct", "Nov", "Dec"};
  time_t t = time(NULL);
  struct tm tm;
  localtime_r(&t, &tm);
  if(t >= (time_t)TIME_VALID_EPOCH) {
    sprintf(timestr, "%02d:%02d", tm.tm_hour, tm.tm_min);
    sprintf(date, "%04d %s %d", tm.tm_year + 1900, mon[tm.tm_mon], tm.tm_mday);
  } else {
    strcpy(timestr, "--:--");
    strcpy(date, "---- --- --");
  }

  u8g2_DrawFrame(&_u8g2, 150, 15, 40, 15);    // battery icon
  u8g2_DrawBox(&_u8g2, 147, 19, 4, 7);
  u8g2_SetFont(&_u8g2, u8g2_font_7x13_tf);
  char vbat[8];
  sprintf(vbat, "%.2fV", 0.0f);
  u8g2_DrawStr(&_u8g2, 153, 27, vbat);

  u8g2_SetFont(&_u8g2, u8g2_font_ncenR18_tr);    // date
  u8g2_DrawStr(&_u8g2, 0, 30, date);

  u8g2_SetFont(&_u8g2, u8g2_font_osb41_tr);    // time
  int width;
  width = u8g2_GetStrWidth(&_u8g2, timestr);
  u8g2_DrawStr(&_u8g2, (200 - width) / 2, 120, timestr);

  char buf[32];    // temp & humidity, WHT20 is 0.01 units
  u8g2_SetFont(&_u8g2, u8g2_font_ncenR24_tr);
  sprintf(buf, "%.1f'C %.1f%%", sensor.T / 100.0f, sensor.RH / 100.0f);
  width = u8g2_GetStrWidth(&_u8g2, buf);
  u8g2_DrawStr(&_u8g2, (200 - width) / 2, 180, buf);
  u8g2_DrawFrame(&_u8g2, 0, 0, 199, 199);
}

void DISP::flush_fb(void)
{
  u8g2_SendBuffer(&_u8g2);
  void u8x8_bitmap_transfer(void (*)(int x1, int y1, int x2, int y2, const void* data));
  SPI.beginTransaction(SPISettings(ZP_SPI_HZ, MSBFIRST, SPI_MODE0));
  u8x8_bitmap_transfer(write_vram);
  SPI.endTransaction();
}

void DISP::update(int fast)
{
  u8g2_ClearBuffer(&_u8g2);
  normal();
  flush_fb();
}

void DISP::show_provision(const char* ap_ssid, const char* ap_pass)
{
  u8g2_ClearBuffer(&_u8g2);
  u8g2_DrawFrame(&_u8g2, 0, 0, 199, 199);
  u8g2_SetFont(&_u8g2, u8g2_font_ncenR24_tr);
  u8g2_DrawStr(&_u8g2, 8, 22, "WiFi setup");
  u8g2_SetFont(&_u8g2, u8g2_font_10x20_tf);
  u8g2_DrawStr(&_u8g2, 8, 55, "Join this AP:");
  u8g2_DrawStr(&_u8g2, 8, 80, ap_ssid ? ap_ssid : "");
  u8g2_DrawStr(&_u8g2, 8, 105, "Password:");
  u8g2_DrawStr(&_u8g2, 8, 130, ap_pass ? ap_pass : "");
  u8g2_DrawStr(&_u8g2, 8, 155, "Then open");
  u8g2_DrawStr(&_u8g2, 8, 180, "http://192.168.4.1");
  flush_fb();
}

void DISP::show_need_setup(void)
{
  u8g2_ClearBuffer(&_u8g2);
  u8g2_DrawFrame(&_u8g2, 0, 0, 199, 199);
  u8g2_SetFont(&_u8g2, u8g2_font_ncenR24_tr);
  u8g2_DrawStr(&_u8g2, 8, 28, "No WiFi");
  u8g2_SetFont(&_u8g2, u8g2_font_10x20_tf);
  u8g2_DrawStr(&_u8g2, 8, 70, "Hold KEY (GPIO6)");
  u8g2_DrawStr(&_u8g2, 8, 100, "and press RESET");
  u8g2_DrawStr(&_u8g2, 8, 130, "to start setup.");
  flush_fb();
}

static const int lcd_hold_pins[] = {ZP_MOSI, ZP_SCK, ZP_CS, ZP_RS, ZP_RESET};

void DISP::hold_io_for_sleep(void)
{
  SPI.end();
  pinMode(ZP_MOSI, OUTPUT);
  pinMode(ZP_SCK, OUTPUT);
  pinMode(ZP_CS, OUTPUT);
  pinMode(ZP_RS, OUTPUT);
  pinMode(ZP_RESET, OUTPUT);
  digitalWrite(ZP_CS, HIGH);
  digitalWrite(ZP_RS, HIGH);
  digitalWrite(ZP_RESET, HIGH);
  digitalWrite(ZP_SCK, LOW);
  digitalWrite(ZP_MOSI, LOW);

  for(int p : lcd_hold_pins) {
    gpio_sleep_sel_dis((gpio_num_t)p);
    gpio_hold_en((gpio_num_t)p);
  }
  gpio_deep_sleep_hold_en();
}

void DISP::init(void)
{
  pinMode(ZP_CS, OUTPUT);
  pinMode(ZP_RS, OUTPUT);
  pinMode(ZP_RESET, OUTPUT);
  pinMode(ZP_BUSY, INPUT);
  digitalWrite(ZP_CS, HIGH);
  digitalWrite(ZP_RS, HIGH);
  digitalWrite(ZP_RESET, HIGH);
  for(int p : lcd_hold_pins)
    gpio_hold_dis((gpio_num_t)p);
  gpio_deep_sleep_hold_dis();

  SPI.begin(ZP_SCK, -1, ZP_MOSI, -1);
  SPI.beginTransaction(SPISettings(ZP_SPI_HZ, MSBFIRST, SPI_MODE0));
  st7305.init();
  SPI.endTransaction();

  u8g2_SetupBitmap(&_u8g2, &u8g2_cb_r0, 200, 200);
  u8x8_InitDisplay(u8g2_GetU8x8(&_u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&_u8g2), 0);

  _state = STATE_NORMAL;
  print_log();
}
