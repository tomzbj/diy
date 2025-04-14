#include "charlie.h"
#include "misc.h"
#include <cstdint>
#include "zpin.h"

void CHARLIE::set_dir(uint32_t port, uint16_t pin, int dir)
{
  if(dir == IN) {
    gpio_mode_set(port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, pin);
  }
  else {
    gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin);
    if(dir == L)
      gpio_bit_set(port, pin);
    else if(dir == H)
      gpio_bit_reset(port, pin);
  }
}

void CHARLIE::view(void)
{
  for(int i = 0; i < 140; i++) {
    printf("(%2d,%1d) ", _array[i] >> 4, _array[i] & 0xf);
    if(i % 10 == 9)
      printf("\n");
  }
  printf("done\n");
}

void CHARLIE::poll(void)
{
  static int count = 0;
  ++count %= 15;
  for(int i = 0; i < 15; i++) {
    set_dir(port[i], pin[i], (count == i) ? L : IN);
  }
  for(int i = 0; i < 140; i++) {
    int x = _array[i] & 0xf;
    int y = _array[i] >> 4;
    if(y != count)
      continue;
    if(_vram[i % 20] & (1 << (i / 20))) {
      set_dir(port[x], pin[x], H);
    }
  }
}

/*void CHARLIE::poll(void)
 {
 static int count = 0;
 ++count %= 140;
 static int prev_x = 0, prev_y = 0;
 set_dir(port[prev_x], pin[prev_x], IN);
 set_dir(port[prev_y], pin[prev_y], IN);
 int x = _array[count] & 0xf;
 int y = _array[count] >> 4;
 if(_vram[count % 20] & (1 << (count / 20))) {
 set_dir(port[x], pin[x], H);
 set_dir(port[y], pin[y], L);
 }
 prev_x = x;
 prev_y = y;
 }*/

void CHARLIE::init(void)
{
  for(int y = 0; y < 15; y++) {
    for(int x = 0; x < 10; x++) {
      if(x < y)
        _array[(y - 1) * 10 + x] = (y << 4) | x;
      else
        _array[(y) * 10 + x] = (y << 4) | x;
    }
  }

  for(int i = 0; i < 15; i++)
    if(i <= 3)
      port[i] = GPIOA;
    else if(i <= 6)
      port[i] = GPIOC;
    else
      port[i] = GPIOB;

  uint16_t tmp[] = {GPIO_PIN_8, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_15, GPIO_PIN_13,
    GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
    GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9};
  memcpy(pin, tmp, sizeof(tmp));

  rcu_periph_clock_enable (RCU_GPIOA);
  rcu_periph_clock_enable (RCU_GPIOB);
  rcu_periph_clock_enable (RCU_GPIOC);
}
