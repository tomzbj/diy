#include "misc.h"
#include "adc_hal.h"
#include <cmath>

#define BUF_SIZE 1024

uint32_t MOISTURE_Get(void)
{
  uint16_t adc_data[BUF_SIZE] = {0};
  ADC_Get(adc_data, BUF_SIZE);

  uint32_t sum = 0;
  for(int i = 0; i < BUF_SIZE; i++) {
    sum += adc_data[i];
//    printf("%d\n", adc_data[i]);
  }
  sum /= BUF_SIZE;

  int16_t dc_data[BUF_SIZE] = {0};
  for(int i = 0; i < BUF_SIZE; i++) {
    dc_data[i] = adc_data[i] - sum;
//    printf("%d\n", dc_data[i]);
  }

  sum = 0;
  for(int i = 0; i < BUF_SIZE; i++) {
    sum += dc_data[i] * dc_data[i];
  }
  sum /= BUF_SIZE;
  sum = sqrt(sum);
  sum = saturate(sum, 0, 1000);
  return sum;
}
