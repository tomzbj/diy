#include <windows.h>
#include <fcntl.h>
#include "misc.h"
#include "app.h"

UDP_SERVER udp_server(8254);
auto udp_write = +[](uint8_t* data, int size) {udp_server.write(data, size);};

const ZUART4::CBS cbs_udp = {    //
  .write_f = udp_write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
ZUART4 udp(cbs_udp, 4096);

static void apply_console(void)
{
  AllocConsole();
  int handle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
  *stdout = *(_fdopen(handle, "w"));
  setvbuf(stdout, NULL, _IONBF, 0);
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int n)
{
  (0) ? apply_console() : (void)0;
  CLI_Config();
  DISP_Config();
  TASKS_Config();
  udp_server.init();
  while(1) {
    zt.poll();
    udp_server.poll();
    if(sdl.update() != 0)
      break;
  }
  udp_server.quit();
  return 0;
}
