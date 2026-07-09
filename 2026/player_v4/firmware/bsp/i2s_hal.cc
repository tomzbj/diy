#include "i2s_hal.h"
#include "platform.h"
#include <array>

static I2S_HandleTypeDef hi2s = {0};
static DMA_HandleTypeDef hdma_i2s_tx = {0};

constexpr int N_SAMPLES = 2048;

int16_t i2s_buf[N_SAMPLES] = {0};
static int16_t i2s_scratch[N_SAMPLES / 2] = {0};

extern class ZFIFO pcm_fifo;

static volatile int g_volume = 9;

const int gain[] = {149, 224, 336, 505, 757, 1136, 1704, 2557, 3835, 5753, 8630, 12945,
  19417, 29126, 43690, 65535};

void I2S_SetVolume(int volume)
{
  g_volume = saturate(volume, 0, 15);
}

void I2S_VolumeUp(void)
{
  g_volume = saturate(g_volume + 1, 0, 15);
}

void I2S_VolumeDown(void)
{
  g_volume = saturate(g_volume - 1, 0, 15);
}

extern "C" void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hi2s.hdmatx);
}

static void get_data_and_adjust_volume(int16_t* target, int n_samples)
{
  const uint32_t want_bytes = (uint32_t)n_samples * sizeof(int16_t);
  memset(i2s_scratch, 0, want_bytes);

  const uint32_t got = pcm_fifo.available() < want_bytes ? pcm_fifo.available() : want_bytes;
  if(got > 0)
    pcm_fifo.get(i2s_scratch, got);

  for(int i = 0; i < n_samples; i++) {
    i2s_scratch[i] = (int16_t)((((int32_t)i2s_scratch[i]) * gain[g_volume]) >> 16);
  }
  memcpy(target, i2s_scratch, want_bytes);
}

extern "C" void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef* hi2s)
{
  get_data_and_adjust_volume(&i2s_buf[N_SAMPLES / 2], N_SAMPLES / 2);
}
extern "C" void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef* hi2s)
{
  get_data_and_adjust_volume(i2s_buf, N_SAMPLES / 2);
}

static void BSP_AUDIO_OUT_ClockConfig(uint32_t freq)
{
  int n, r;
  switch(freq) {
    case 11025:
    case 22050:
    case 44100:
      n = 271;
      r = 2;
      break;
    case 96000:
      n = 344;
      r = 2;
      break;
    default:
      n = 344;
      r = 7;
      break;
  }
  RCC_PeriphCLKInitTypeDef rpckis = {0};
  HAL_RCCEx_GetPeriphCLKConfig(&rpckis);
  rpckis.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1 | RCC_PERIPHCLK_PLLI2S;
  rpckis.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
  rpckis.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;
  rpckis.PLLI2S.PLLI2SM = 16;
  rpckis.PLLI2S.PLLI2SN = n;
  rpckis.PLLI2S.PLLI2SR = r;
  HAL_RCCEx_PeriphCLKConfig(&rpckis);
}

void I2S_SetSampleRate(uint32_t sample_rate)
{
  BSP_AUDIO_OUT_ClockConfig(sample_rate);
  __HAL_I2S_DISABLE(&hi2s);
  hi2s.Init.AudioFreq = sample_rate;
  HAL_I2S_Init(&hi2s);
  __HAL_I2S_ENABLE(&hi2s);
}

void I2S_Config(void)
{
  __HAL_RCC_SPI1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  __HAL_I2S_DISABLE(&hi2s);
  HAL_I2S_DeInit(&hi2s);

  GPIO_InitTypeDef gis = {0};

  gis.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
  gis.Mode = GPIO_MODE_AF_PP;
  gis.Pull = GPIO_NOPULL;
  gis.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gis.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &gis);
  gis.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOC, &gis);

  hi2s.Instance = SPI1;
  __HAL_I2S_DISABLE(&hi2s);
  hi2s.Init.AudioFreq = 12000;
  hi2s.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s.Init.CPOL = I2S_CPOL_LOW;
  hi2s.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  HAL_I2S_Init(&hi2s);
  __HAL_I2S_ENABLE(&hi2s);

  I2S_SetSampleRate(12000);

  hdma_i2s_tx.Init.Channel = DMA_CHANNEL_2;
  hdma_i2s_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_i2s_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_i2s_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_i2s_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_i2s_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_i2s_tx.Init.Mode = DMA_CIRCULAR;
  hdma_i2s_tx.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_i2s_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_i2s_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_i2s_tx.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_i2s_tx.Init.PeriphBurst = DMA_MBURST_SINGLE;
  hdma_i2s_tx.Instance = DMA2_Stream2;
  __HAL_LINKDMA(&hi2s, hdmatx, hdma_i2s_tx);
  HAL_DMA_DeInit(&hdma_i2s_tx);
  HAL_DMA_Init(&hdma_i2s_tx);
  HAL_NVIC_SetPriority(SPI1_IRQn, 0x0F, 0x00);
  HAL_NVIC_EnableIRQ(SPI1_IRQn);
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0xe, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

  HAL_I2S_Transmit_DMA(&hi2s, (uint16_t*)i2s_buf, N_SAMPLES);

  print_log();
}
