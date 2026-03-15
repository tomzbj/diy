#include "zdds.h"
#include "dds.h"
#include "misc.h"
#include "platform.h"
#include "dtmf.h"

ZDDS::cbs_t cbs = {DAC_DDS_Start, DAC_DDS_Stop};
ZDDS dds(cbs);
DTMF_MOD mod(dds, 200, 15);

DTMF_DEM dem(8000, 205);

void DDS_Update(void)
{
}

void DDS_Config(void)
{
  dds.set_sample_rate(100000UL);
  dds.start();
  dds.set_freq(1, 1);
  print_log();
}
