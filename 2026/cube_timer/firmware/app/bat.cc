#include "bat.h"
#include "adc_hal.h"
#include "ui.h"
#include "misc.h"

// Levels: 0 EMPTY .. 4 FULL
// Fall at nominal edges; rise needs +HYST (15mV) to avoid chatter
static constexpr float HYST = 0.015f;
static constexpr float TH_FULL = 4.10f;
static constexpr float TH_3 = 3.90f;
static constexpr float TH_2 = 3.70f;
static constexpr float TH_1 = 3.50f;
static constexpr float TH_EMPTY = 3.30f;

static int g_level = 4;

static int bat_level_update(float v, int prev)
{
  switch(prev) {
    case 4:    // FULL
      if(v < TH_FULL - HYST)
        return 3;
      return 4;
    case 3:
      if(v >= TH_FULL + HYST)
        return 4;
      if(v < TH_3 - HYST)
        return 2;
      return 3;
    case 2:
      if(v >= TH_3 + HYST)
        return 3;
      if(v < TH_2 - HYST)
        return 1;
      return 2;
    case 1:
      if(v >= TH_2 + HYST)
        return 2;
      if(v < TH_1 - HYST)
        return 0;
      return 1;
    default:    // EMPTY
      if(v >= TH_1 + HYST)
        return 1;
      return 0;
  }
}

static int bat_level_init(float v)
{
  if(v >= TH_FULL)
    return 4;
  if(v >= TH_3)
    return 3;
  if(v >= TH_2)
    return 2;
  if(v >= TH_1)
    return 1;
  return 0;
}

void BAT_Poll(void)
{
  float v = ADC_ReadVbat();
  g_level = bat_level_update(v, g_level);
  UI_SetBatteryLevel(g_level);
  printf("vbat=%.3f L%d\n", v, g_level);
}

void BAT_Config(void)
{
  ADC_DividerEnable(1);
  float v = ADC_ReadVbat();
  g_level = bat_level_init(v);
  UI_SetBatteryLevel(g_level);
  printf("vbat=%.3f L%d (init)\n", v, g_level);
  print_log();
}
