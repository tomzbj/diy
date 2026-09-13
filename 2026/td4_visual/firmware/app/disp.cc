#include "misc.h"
#include "platform.h"
#include "st7789.h"
#include "disp.h"

auto setcs = [](int s) {ZPin::write(PA15, s);};
auto setrs = [](int r) {ZPin::write(PB4, r);};    // for ser mode
auto reset = [](int n) {ZPin::write(PB6, n);};

//static TFT::cbs_t cbs = {reset, setcs, setrs, (void*)SPI1_Write, _delay_us};
static TFT::cbs_t cbs = {reset, setcs, setrs, (void*)SPI1_Write_DMA, _delay_us};
TFT_ST7789 tft(240, 280, TFT::DATAWIDTH_8, cbs);

static void pset(UG_S16 x, UG_S16 y, UG_COLOR c)
{
  tft.set_window(x, y, x, y);
  uint16_t px = c;
  tft.write(&px, 1);
}

/* HV lines only; diagonal → FAIL so µGUI falls back to pset. */
static UG_RESULT driver_drawline(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
  if(x1 != x2 && y1 != y2)
    return UG_RESULT_FAIL;

  if(x1 > x2) {
    UG_S16 t = x1;
    x1 = x2;
    x2 = t;
  }
  if(y1 > y2) {
    UG_S16 t = y1;
    y1 = y2;
    y2 = t;
  }

  int count = (x2 - x1 + 1) * (y2 - y1 + 1);
  uint16_t line_buf[320];
  if(count > 320)
    return UG_RESULT_FAIL;
  for(int i = 0; i < count; i++)
    line_buf[i] = c;

  tft.set_window(x1, y1, x2, y2);
  tft.write(line_buf, count);
  return UG_RESULT_OK;
}

static UG_RESULT driver_fill_frame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2,
  UG_COLOR c)
{
  /* UG_FillFrame already sorts x1<=x2, y1<=y2. */
  constexpr int BUF_SIZE = 256;
  uint16_t frame_buf[BUF_SIZE];
  for(int i = 0; i < BUF_SIZE; i++)
    frame_buf[i] = c;

  int count = (y2 - y1 + 1) * (x2 - x1 + 1);
  tft.set_window(x1, y1, x2, y2);
  while(count > 0) {
    int n = (count >= BUF_SIZE) ? BUF_SIZE : count;
    tft.write(frame_buf, n);
    count -= n;
  }
  return UG_RESULT_OK;
}

/* FILL_AREA: set_window once, then stream pixels with CS held low.
 * Do NOT call tft.write() here — it toggles CS and byte-swaps again. */
static void push_pixel(UG_COLOR c)
{
  uint16_t be = (uint16_t)((c << 8) | (c >> 8));
  SPI1_Write(&be, 2);
}

static void* driver_fill_area(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2)
{
  tft.set_window(x1, y1, x2, y2);
  setcs(0);
  setrs(1);
  return (void*)push_pixel;
}

void DISP::init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitTypeDef gis = {0};
  gis.Pin = GPIO_PIN_4 | GPIO_PIN_6;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gis);
  gis.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Pin = GPIO_PIN_13;    // LCD_BK_CTL
  gis.Mode = GPIO_MODE_OUTPUT_OD;
  gis.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &gis);
  ZPin::set(PC13);

  tft.init();
//  tft.clear(0xffff);
//  tft.clear(0x0);

  UG_Init(&_gui, pset, 240, 280);
  UG_DriverRegister(DRIVER_DRAW_LINE, (void*)driver_drawline);
  UG_DriverRegister(DRIVER_FILL_FRAME, (void*)driver_fill_frame);
  UG_DriverRegister(DRIVER_FILL_AREA, (void*)driver_fill_area);

  UG_SelectGUI(&_gui);
  UG_FillScreen(C_BLACK);
  UG_FontSetHSpace(0);
  UG_FontSetVSpace(0);
  UG_FontSelect(&FONT_12X16);
//  UG_SetForecolor(C_BLACK);
//  UG_SetBackcolor(C_WHITE);
  UG_SetBackcolor(C_BLACK);
  print_log();

  update();
}

void DISP::update(void)
{
  uint8_t ins[16];
  rom.read_instructions(ins, 16);

  const char* ins_name[] = {"ADD A, ", "MOV A, B", "IN A", "MOV A, ", "MOV B, A",
    "ADD B, ", "IN B", "MOV B, ", "", "OUT B", "", "OUT ", "", "", "JNC ", "JMP "};

  UG_SetForecolor(C_WHITE);
  for(int i = 0; i < 16; i++) {
//    if(_prev_ins[i] == ins[i]) continue;
    char buf[20] = {0}, buf2[8] = {0};
    auto cur_ins = ins[i] >> 4;
    auto cur_im = ins[i] & 0xf;
    sprintf(buf, "%02d %02X %s", i, ins[i], ins_name[cur_ins]);
    switch(cur_ins) {
      case 0:
      case 3:
      case 5:
      case 7:
      case 11:
      case 14:
      case 15:
        sprintf(buf2, "%d", cur_im);
        strcat(buf, buf2);
        break;
      default:
        break;
    }
    while(strlen(buf) < 18)
      strcat(buf, " ");
    UG_PutString(20, i * 17 + 4, buf);
  }
  if(_state == MOVE || _state == EDIT) {
    char buf[4] = {0};
    auto i = _cursor_pos / 2;
    auto ins_h = ins[i] >> 4;
    auto ins_l = ins[i] & 0xf;

    if(_state == MOVE)
      UG_SetForecolor(C_GREEN);
    else
      UG_SetForecolor(C_RED);

    if(_cursor_pos & 1) {
      sprintf(buf, "%X", ins_l);
      UG_PutString(68, i * 17 + 4, buf);
    }
    else {
      sprintf(buf, "%X", ins_h);
      UG_PutString(56, i * 17 + 4, buf);
    }
  }
//  memcpy(_prev_ins, ins, 16);
  _prev_cursor_pos = _cursor_pos;
  _prev_state = _state;
}

constexpr int hash(DISP::STATE state, DISP::ACTION action)
{
  return state * 10 + action;
}

static void edit_rom(int cursor_pos, int direction)
{
  uint8_t ins[16];
  rom.read_instructions(ins, 16);
  int index = cursor_pos / 2;
//printf("cursor_pos: %d\n", cursor_pos);
  auto& n = ins[index];
  if(cursor_pos & 1) {    // low 4bits
    if(direction)
      n = (n & 0xf0) | ((n + 1) & 0xf);
    else
      n = (n & 0xf0) | ((n - 1) & 0xf);
  }
  else {    // high 4bits
    if(direction)
      n = (n & 0x0f) | ((n + 0x10) & 0xf0);
    else
      n = (n & 0x0f) | ((n - 0x10) & 0xf0);
  }
  rom.write_instructions(ins, 16);
}

void DISP::go(ACTION action)
{

  switch(hash(_state, action)) {
    // normal
    case hash(NORMAL, KEY_DOWN_LONGPRESS):
      _state = MOVE;
      break;

      // move
    case hash(MOVE, KEY_DOWN_LONGPRESS):
      _state = EDIT;
      break;
    case hash(MOVE, KEY_UP_LONGPRESS):
      _state = NORMAL;
      break;
    case hash(MOVE, KEY_UP_PRESS):
      _cursor_pos += 31;
      _cursor_pos %= 32;
      break;
    case hash(MOVE, KEY_DOWN_PRESS):
      _cursor_pos++;
      _cursor_pos %= 32;
      break;

      // edit: short press adjusts nibble; long-down commits; long-up reverts
    case hash(EDIT, KEY_UP_PRESS):
      edit_rom(_cursor_pos, 0);
      break;
    case hash(EDIT, KEY_DOWN_PRESS):
      edit_rom(_cursor_pos, 1);
      break;
    case hash(EDIT, KEY_DOWN_LONGPRESS):
      rom.save();
      _state = MOVE;
      break;
    case hash(EDIT, KEY_UP_LONGPRESS): {
      rom.load();
      _state = MOVE;
      break;
    }
  }
  update();
}
