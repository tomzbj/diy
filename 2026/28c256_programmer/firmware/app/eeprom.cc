#include "eeprom.h"
#include "misc.h"
#include "platform.h"

// AT28CXX cbs
#define ZP_OE PB15
#define ZP_WE PA8
#define ZP_CE PC13

static void write_addr(uint32_t addr);
static void write_data(uint8_t data);
static uint8_t read_data(void);

auto setoe = +[](int s) {ZPin::write(ZP_OE, s);};
auto setwe = +[](int s) {ZPin::write(ZP_WE, s);};
auto setce = +[](int s) {ZPin::write(ZP_CE, s);};

AT28CXX::CBS cbs = {
//
  .setoe_f = setoe, .setwe_f = setwe, .setce_f = setce,    //
  .read_data_f = read_data, .write_data_f = write_data,    //
  .write_addr_f = write_addr, .delay_us_f = _delay_us    //
  };

AT28CXX at28cxx(cbs);

// FLASHER cbs
int read_f(uint32_t addr, int size, void* buf)
{
  at28cxx.read(addr, (uint8_t*)buf, size);
  return 0;
}
int write_f(uint32_t addr, int size, const void* buf)
{
  at28cxx.write(addr, (const uint8_t*)buf, size);
  return 0;
}

extern "C" void uwrite_f(const void* data, int size)
{
  extern class ZUART4 u1;
  u1.write(data, size);
//  void USB_Write(uint8_t* data, int size);
//  USB_Write((uint8_t*)data, size);
}

extern "C" void usb_write_f(const void* data, int size)
{
  void USB_Write(uint8_t* data, int size);
  USB_Write((uint8_t*)data, size);
}

FLASHER_AT28C::CBS cbs_flasher = {    //
  .read_f = read_f, .write_f = write_f, .usart_write_f = uwrite_f};
FLASHER_AT28C::CBS cbs_flasher_u = {    //
  .read_f = read_f, .write_f = write_f, .usart_write_f = usb_write_f};

FLASHER_AT28C flasher(cbs_flasher, 32768, 2000);
FLASHER_AT28C flasher_u(cbs_flasher_u, 32768, 2000);

static void write_addr(uint32_t addr)
{
//_dbg();
  GPIOB->ODR = (GPIOB->ODR & 0x8000) | addr;
}

static void write_data(uint8_t data)
{
//_dbg();

  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5
    | GPIO_PIN_6 | GPIO_PIN_7;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);

  GPIOA->ODR = (GPIOA->ODR & 0xff00) | data;
}

static uint8_t read_data(void)
{
  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5
    | GPIO_PIN_6 | GPIO_PIN_7;
  gis.Mode = GPIO_MODE_INPUT;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gis);

  return GPIOA->IDR & 0xff;
}

static void eeprom_gpio_init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef gis = {0};
  // GPIOB as addr
  gis.Pin = GPIO_PIN_All;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);
  // GPIOA as data
  gis.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5
    | GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &gis);

  // PC13 as CE, PB15 as OE, PA8 as WE
  gis.Pin = GPIO_PIN_13;
  HAL_GPIO_Init(GPIOC, &gis);
  gis.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &gis);
  gis.Pin = GPIO_PIN_8;
  HAL_GPIO_Init(GPIOA, &gis);
  setoe(1);
  setce(1);
  setwe(1);
}

void EEPROM_Config(void)
{
  eeprom_gpio_init();
//  flasher.init();
//  printf("%02x\n", at28cxx.read_byte(0x00000));
//  printf("%04x\n", at28cxx.read_id());
  print_log();
}
