#include "misc.h"

unsigned char SPI0_ReadWrite(unsigned char data)
{
    while(spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET);
    spi_i2s_data_transmit(SPI0, data);
    while(spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE) == RESET);
    return spi_i2s_data_receive(SPI0);
}

void SPI_Config(void)
{
    rcu_periph_clock_enable (RCU_SPI0);

    spi_parameter_struct sis;
    spi_i2s_deinit (SPI0);

    spi_struct_para_init(&sis);

    sis.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    sis.device_mode = SPI_MASTER;
    sis.frame_size = SPI_FRAMESIZE_8BIT;
    sis.nss = SPI_NSS_SOFT;
    sis.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    sis.prescale = SPI_PSC_8;
    sis.endian = SPI_ENDIAN_MSB;

    spi_init(SPI0, &sis);
    spi_enable(SPI0);

    _dbg();
}
