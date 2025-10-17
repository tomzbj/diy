#include "misc.h"
#include "platform.h"
#include "disp.h"
#include "ssd1306_i2c.h"
#include "u8g2.h"
#include "mui.h"
#include "mui_u8g2.h"

// PC13/14/15 as reset, scl, sda
u8g2_t u8g2;
mui_t ui;

uint16_t tune = 13;
uint8_t bpf = 4, vol = 8;

uint16_t animals_get_cnt(void* data)
{
  return 121;
}

const char* animals_get_str(void* data, uint16_t index)
{
  static char str[8];
  sprintf(str, "%d", index * 9 + 522);
  return str;
}

muif_t muif_list[] MUI_PROGMEM = {
  MUIF_U8G2_LABEL(),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_ncenB08_tr),
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_6x12_tr),
  MUIF_U8G2_FONT_STYLE(2, u8g2_font_spleen8x16_mr),
  MUIF_U8G2_U16_LIST("TN", &tune, NULL, animals_get_str, animals_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),

  MUIF_VARIABLE("BP", &bpf,mui_u8g2_u8_opt_line_wa_mud_pi),
  MUIF_VARIABLE("VO", &vol,mui_u8g2_u8_opt_line_wa_mud_pi),

  MUIF_BUTTON("G0", mui_u8g2_btn_goto_wm_fi)
};

fds_t fds[] MUI_PROGMEM =
MUI_FORM(1)
MUI_STYLE(2)
MUI_LABEL(0,16, "TUNE")
MUI_LABEL(52,16, "BPF")
MUI_LABEL(104,16, "VOL.")
MUI_XYAT("TN", 0, 31, 40, "Banana|Apple|Melon|Cranberry")
MUI_XYAT("BP", 52, 31, 40, "200|500|1k|2k|5k|10k|20k")
MUI_XYAT("VO", 104, 31, 40, "3|4|5|6|7|8|9|10|11|12|13|14")
;

static uint8_t cb_gpio_delay(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr)
{
  switch(msg) {
    case U8X8_MSG_DELAY_MILLI:    // delay arg_int * 1 milli second
      _delay_ms(1);
      break;
    case U8X8_MSG_DELAY_I2C:     // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
      _delay_us(0);
      break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK:    // arg_int=0: Output low at I2C clock pin
      ZPin::write(PC14, arg_int);
      break;                  // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:    // arg_int=0: Output low at I2C data pin
      ZPin::write(PC15, arg_int);
      break;                   // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
//      u8x8_SetGPIOResult(u8x8, /* get menu select pin state */0);
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, !ZPin::read(PB7));     ///* get menu next pin state */0);
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, !ZPin::read(PB8));     ///* get menu prev pin state */0);
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
//      u8x8_SetGPIOResult(u8x8, /* get menu home pin state */0);
      break;
    default:
      u8x8_SetGPIOResult(u8x8, 1);    // default return value
      break;
  }
  return 1;
}

void DISP_Config(void)
{
  rcu_periph_clock_enable (RCU_GPIOC);
  auto pins = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, pins);
  gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pins);
  pins = GPIO_PIN_15;
  gpio_output_options_set(GPIOC, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, pins);
  ZPin::clear (PC13);
  _delay_ms(3);
  ZPin::set(PC13);
  _delay_ms(3);

  // Initialization
  u8g2_Setup_ssd1306_i2c_128x64_alt0_f(&u8g2, U8G2_R2, u8x8_byte_sw_i2c, cb_gpio_delay);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);

  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list) / sizeof(muif_t));
  mui_GotoForm(&ui, 1, 0);

  DISP_Update();
  print_log();
}

void DISP_Update(void)
{
  u8g2_ClearBuffer(&u8g2);
  mui_Draw(&ui);
  u8g2_SendBuffer(&u8g2);
}
