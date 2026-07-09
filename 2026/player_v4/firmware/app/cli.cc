#include "misc.h"
#include "platform.h"
#include "fs_lock.h"
#include <malloc.h>

//#include "wav4.h"

#define ok() printf("ok\n");

ZCli cli(40);

static void cli_wr_sd(const char* tokens[])
{
//  uint8_t BSP_SD_ReadBlocks(uint32_t* pData, uint32_t addr, uint32_t nblocks, uint32_t Timeout)
  int addr = atoi(tokens[2]);
  uint8_t seed = atoi(tokens[3]);
  int nblocks = 1;    //atoi(tokens[3]);
  uint8_t buf[512] = {0};
  for(int i = 0; i < 512; i++) {
    buf[i] = seed++;
  }
  int ret = BSP_SD_WriteBlocks((uint32_t*)buf, addr, nblocks, 100);
  ok();
  printf("ret: %d\n", ret);
}

static void cli_rd_sd(const char* tokens[])
{
//  uint8_t BSP_SD_ReadBlocks(uint32_t* pData, uint32_t addr, uint32_t nblocks, uint32_t Timeout)
  int addr = atoi(tokens[2]);
  int nblocks = 1;    //atoi(tokens[3]);
  uint8_t buf[512] = {0};
  int ret = BSP_SD_ReadBlocks((uint32_t*)buf, addr, nblocks, 100);

  for(int i = 0; i < 512; i++) {
    printf("%02x ", buf[i]);
    if(i % 16 == 15)
      printf("\n");
  }
  ok();
  printf("ret: %d\n", ret);
}

static void cli_wr_off(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gis;
  gis.Pin = GPIO_PIN_15;
  gis.Mode = GPIO_MODE_OUTPUT_PP;
  gis.Pull = GPIO_PULLUP;
  gis.Speed = GPIO_SPEED_FREQ_HIGH;
//  gis.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &gis);
  ZPin::clear(PA15);
}

static void cli_rd_cardinfo(void)
{
  extern SD_HandleTypeDef hsd;
  HAL_SD_CardCIDTypeDef cid;
  HAL_SD_CardCSDTypeDef csd;
  HAL_SD_CardStatusTypeDef cardstatus;
  HAL_SD_CardInfoTypeDef info;

  auto state = HAL_SD_GetCardState(&hsd);
  printf("%lu\n", state);
  auto status = HAL_SD_GetCardCID(&hsd, &cid);
  printf("%d\n", status);
  printf("%3u %5u %8lu %3u %3u %8lu %5u %3u\n", cid.ManufacturerID, cid.OEM_AppliID,
    cid.ProdName1, cid.ProdName2, cid.ProdRev, cid.ProdSN, cid.ManufactDate,
    cid.CID_CRC);
//    __IO uint8_t  ManufacturerID;  /*!< Manufacturer ID       */
//  __IO uint16_t OEM_AppliID;     /*!< OEM/Application ID    */
//  __IO uint32_t ProdName1;       /*!< Product Name part1    */
//  __IO uint8_t  ProdName2;       /*!< Product Name part2    */
//  __IO uint8_t  ProdRev;         /*!< Product Revision      */
//  __IO uint32_t ProdSN;          /*!< Product Serial Number */
//  __IO uint8_t  Reserved1;       /*!< Reserved1             */
//  __IO uint16_t ManufactDate;    /*!< Manufacturing Date    */
//  __IO uint8_t  CID_CRC;         /*!< CID CRC               */
//  __IO uint8_t  Reserved2;       /*!< Always 1              */
  status = HAL_SD_GetCardCSD(&hsd, &csd);

  printf("%d\n", status);
  status = HAL_SD_GetCardStatus(&hsd, &cardstatus);
  printf("%d\n", status);
  {
    auto p = &cardstatus;
    printf("%3u %3u %5u %8lu %3u %3u %3u %5u %3u %3u\n", p->DataBusWidth,
      p->SecuredMode, p->CardType, p->ProtectedAreaSize, p->SpeedClass,
      p->PerformanceMove, p->AllocationUnitSize, p->EraseSize, p->EraseTimeout,
      p->EraseOffset);
  }
//  __IO uint8_t  DataBusWidth;           /*!< Shows the currently defined data bus width                 */
//  __IO uint8_t  SecuredMode;            /*!< Card is in secured mode of operation                       */
//  __IO uint16_t CardType;               /*!< Carries information about card type                        */
//  __IO uint32_t ProtectedAreaSize;      /*!< Carries information about the capacity of protected area   */
//  __IO uint8_t  SpeedClass;             /*!< Carries information about the speed class of the card      */
//  __IO uint8_t  PerformanceMove;        /*!< Carries information about the card's performance move      */
//  __IO uint8_t  AllocationUnitSize;     /*!< Carries information about the card's allocation unit size  */
//  __IO uint16_t EraseSize;              /*!< Determines the number of AUs to be erased in one operation */
//  __IO uint8_t  EraseTimeout;           /*!< Determines the timeout for any number of AU erase          */
//  __IO uint8_t  EraseOffset;            /*!< Carries information about the erase offset                 */

  status = HAL_SD_GetCardInfo(&hsd, &info);
  printf("%d\n", status);
  {
    auto p = &info;
    printf("%8lu %8lu %8lu %8lu %8lu %8lu %8lu %8lu\n", p->CardType, p->CardVersion,
      p->Class, p->RelCardAdd, p->BlockNbr, p->BlockSize, p->LogBlockNbr,
      p->LogBlockSize);
  }

}

static void cli_rd_bootcnt(void)
{
  FS_LOCK lock;
  int err;
  uint32_t boot_count = 0;
  FIL file;
  unsigned int n;
  err = f_open(&file, "boot_cnt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
  err = f_read(&file, &boot_count, sizeof(boot_count), &n);
  printf("boot_count: %lu\n", boot_count);
  boot_count++;
  err = f_rewind(&file);
  err = f_write(&file, &boot_count, sizeof(boot_count), &n);
  err = f_close(&file);
  err = err;
}

static void cli_wr_volume(const char* tokens[])
{
  int volume = atoi(tokens[2]);
  I2S_SetVolume(volume);
  ok();
}
/*static void cli_play_wav(const char* tokens[]) {
 printf("%lu\n", wav4_size);
 I2S_PlayWav(wav4, wav4_size ); ok(); }*/

static void cli_rd_mallinfo(void)
{
  register uint8_t* sp asm("sp");
  auto a2 = sp;
  auto a1 = malloc(1);
  printf("%08lx %08lx\n", (uint32_t)a1, (uint32_t)a2);
  if(a2 != nullptr)
    free(a1);

  struct mallinfo mi = mallinfo();
  printf("Arena: %d\n", mi.arena);
  printf("Used: %d, Free: %d\n", mi.uordblks, mi.fordblks);

  struct _reent* r = _impure_ptr;
  printf("Reent pointer: %08lx\n", (uint32_t)(void*)r);
  printf("Reent size: %d\n", sizeof(struct _reent));

  // 如果 _stdout 不为空，说明标准 I/O 已经初始化并占用了内存
  if(r->_stdout != NULL) {
    printf("stdout is initialized at %08lx\n", (uint32_t)((void*)r->_stdout));
  }
}

void CLI_Config(void)
{
  cli.bind("test", []() {puts("TEST OK");});
  cli.bind("reboot", []() {puts("REBOOT."); NVIC_SystemReset();});
  cli.bind("freq", "rd", []() {printf("%lu\n", HAL_RCC_GetSysClockFreq());});
  cli.bind("off", "wr", cli_wr_off);
  cli.bind("sd", "rd", cli_rd_sd);
  cli.bind("sd", "wr", cli_wr_sd);
  cli.bind("info", "rd", cli_rd_cardinfo);
  cli.bind("bootcnt", "rd", cli_rd_bootcnt);

  extern class FS fs;
  cli.bind("info", [](const char** tokens) {fs.cmd_info(tokens);});
  cli.bind("ls", [](const char** tokens) {fs.cmd_ls(tokens);});
  cli.bind("ll", [](const char** tokens) {fs.cmd_ls(tokens);});
  cli.bind("cat", [](const char** tokens) {fs.cmd_cat(tokens);});
  cli.bind("pwd", [](const char** tokens) {fs.cmd_pwd(tokens);});
  cli.bind("cd", [](const char** tokens) {fs.cmd_cd(tokens);});
  cli.bind("touch", [](const char** tokens) {fs.cmd_touch(tokens);});
  cli.bind("hexview", [](const char** tokens) {fs.cmd_hexview(tokens);});
  cli.bind("crc32", [](const char** tokens) {fs.cmd_crc32(tokens);});
  cli.bind("rm", [](const char** tokens) {fs.cmd_rm(tokens);});
  cli.bind("rename", [](const char** tokens) {fs.cmd_rename(tokens);});
  cli.bind("mv", [](const char** tokens) {fs.cmd_rename(tokens);});

  cli.bind("mkfs", [](const char** tokens) {
    (void)tokens;
    FS_LOCK lock;
    extern FATFS fatfs;
    static uint8_t workbuf[512];
    int ret = f_mkfs("0:", FM_FAT32 | FM_SFD, 512, workbuf, sizeof(workbuf));
    f_mount(&fatfs, "0:", 1);
    printf("ret: %d\n", ret);
  });

//  cli.bind("playwav", "wr", cli_play_wav);
  cli.bind("play", [](const char** tokens) {
    if(strlen(tokens[1]) > 30) return;
    char buf[258]= {0};
    sprintf(buf, "%s/%s", fs.pwd(), tokens[1]);
//    printf("#%s#\n", tokens[1]);
    player.set_file(buf);
  });

  cli.bind("volume", "wr", cli_wr_volume);
  cli.bind("mallinfo", "rd", cli_rd_mallinfo);
  cli.bind("t", "rd", []() {
//    extern uint32_t _t;
//    printf("%lu\n", _t);
  });

  print_log();
}
