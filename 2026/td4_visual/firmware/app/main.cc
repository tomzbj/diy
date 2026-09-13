#include "misc.h"
#include "platform.h"

const ZUART4::cbs_t cbs_u1 = {    //
  .write_f = USART1_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUART4 u1(cbs_u1, 512);

static void rom_init_f(void);
auto addr_in_f = +[](void) -> uint8_t {return GPIOB->IDR >> 12;};
auto data_out_f = +[](uint8_t data) {GPIOA->ODR = (GPIOA->ODR & 0xff00) | data;};
auto load_f = +[](uint8_t* data, int size) {EEPROM_Read((void*)data, size);};
auto save_f = +[](const uint8_t* data, int size) {    //
    EEPROM_Write((const void*)data, size);};
const ROM::CBS cbs_rom = {    //
  .init_f = rom_init_f, .addr_in_f = addr_in_f, .data_out_f = data_out_f,    //
    .printf_f = printf, .load_f = load_f, .save_f = save_f};

ROM rom(cbs_rom);
DISP disp;

static void rom_init_f(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);
  gis.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5
    | GPIO_PIN_6 | GPIO_PIN_7;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &gis);
}

int main(void)
{
  SysTick_Config(HAL_RCC_GetSysClockFreq() / 1000UL);
  USART1_Config();
  SPI1_Config();
  CLI_Config();
  rom.init();

  disp.init();
  KEY_Config();
  TASKS_Config();

  printf("SYSCLK: %lu\n", HAL_RCC_GetSysClockFreq());

  while(1) {
    _delay_us(1);
    zt.poll();
  }
  return 0;
}
