#include "misc.h"
#include <cstdarg>
#include "app.h"

#define ok() udp_printf("ok\n");

ZCli cli(40);

static int udp_printf(const char* fmt, ...)
{
  char buf[256] = {0};
  va_list arp;
  va_start(arp, fmt);
  int n = vsnprintf(buf, sizeof(buf), fmt, arp);
  udp_server.write((uint8_t*)buf, n);
  va_end(arp);
  return n;
}

static void cli_rd_sbrk_sp(const char* tokens[])
{
  register uint8_t* sp asm("sp");
  auto a1 = sp;
  auto a2 = malloc(1);
  udp_printf("%p %p %d\n", a1, a2, (int)a1 - (int)a2);
  free(a2);
}

static void cli_wr_switch_page(const char* tokens[])
{
  extern class UI ui;
  auto page = static_cast<UI::PAGE>(atoi(tokens[2]));
  if(page < ui.PAGE::MIN || page > ui.PAGE::MAX)
    return;
  ui.switch_page(page);
  ok();
}

static void cli_wr_vals(const char* tokens[])
{
//  extern UI::vals_t vals;
//  vals.period = atof(tokens[2]);
//  vals.resistance = atof(tokens[3]);
  ok();
}

static void cli_quit(const char* tokens[])
{
  SDL_Event event = {0};

  event.type = SDL_QUIT;    // 设置事件类型
  event.user.code = 0;    // 自定义代码
  event.user.data1 = NULL;    // 自定义数据指针1
  event.user.data2 = NULL;    // 自定义数据指针2
  SDL_PushEvent(&event);
  ok();
}

static void cli_rd_mem(void)
{
  lv_mem_monitor_t mon;
  lv_mem_monitor(&mon);

  udp_printf("total heap: %d bytes\n", (int)mon.total_size);
  udp_printf("used heap: %d bytes (percent: %d %%)\n",    //
    (int)(mon.total_size - mon.free_size), mon.used_pct);
  udp_printf("max continuous heap: %d bytes\n", (int)mon.free_biggest_size);
  udp_printf("fragment percent: %d %%\n", mon.frag_pct);
}

/* @formatter:off */
const auto test_f = [](const char**) {udp_printf("TEST OK\n");};
const auto build_datetime_f = []( const char**) {udp_printf("%s %s\n", __DATE__, __TIME__);};
/* @formatter:on */

void CLI_Config(void)
{
  // misc functions
  cli.bind("test", test_f);
  cli.bind("reboot", cli_quit);
  cli.bind("build_datetime", build_datetime_f);
  cli.bind("vals", "wr", cli_wr_vals);    // comm functions
  cli.bind("sbrk_sp", "rd", cli_rd_sbrk_sp);    // mem functions
  cli.bind("switch_page", "wr", cli_wr_switch_page);    // disp functions

  // vals
  /*  void vals_inc_pos(void); void vals_dec_pos(void); void vals_inc(void);
   void vals_dec(void); cli.bind("pos_inc", "wr", [](const char**) {vals_inc_pos();});
   cli.bind("pos_dec", "wr", [](const char**) {vals_dec_pos();});
   cli.bind("val_dec", "wr", [](const char**) {vals_dec();});
   cli.bind("val_inc", "wr", [](const char**) {vals_inc();});*/
  cli.bind("mem", "rd", cli_rd_mem);

  _dbg();
}
