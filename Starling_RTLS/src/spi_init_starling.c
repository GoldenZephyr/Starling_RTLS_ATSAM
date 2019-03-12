#include "spi_init_starling.h"
#include "pio.h"
#include "spi.h"
#include "sysclk.h"
#include "asf.h"

#define SPI_ID            ID_SPI
#define SPI_MASTER_BASE     SPI
#define SPI_MISO_GPIO  (PIO_PA12_IDX)
#define SPI_MISO_FLAGS (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_MOSI_GPIO  (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_SPCK_GPIO  (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS0_GPIO            (PIO_PA11_IDX)
#define SPI_CHIP_SEL 0
#define SPI_NPCS0_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 0

/* Delay before SPCK. */
//#define SPI_DLYBS 0x40
#define SPI_DLYBS 0xFF

/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0x10
/* SPI clock setting (Hz). */
static uint32_t gs_ul_spi_clock = 2000000;

//Paramteres:
//  data: data to be sent
//  last_byte: either 0 or 1.
//   0-There are following bytes to be sent. The following bytes and the current byte composes one data-unit, CS should be kept low after sending out the current byte
//   1-Last byte of a data unit. CS will be set to HIGH when the current byte has been sent out.
void spi_tx(uint8_t data,uint8_t last_byte)
{
	spi_write(SPI,data,SPI_CHIP_SEL,last_byte);
	while ((spi_read_status(SPI) & SPI_SR_RDRF) == 0);/* Wait transfer done. */
}

uint8_t spi_rx(void)
{
	uint8_t tmp;
	spi_read(SPI,&tmp,SPI_CHIP_SEL);
	return tmp;
}

void spi_init(void)
{
	//Assign I/O lines to peripheral
	gpio_configure_pin(SPI_MISO_GPIO,SPI_MISO_FLAGS);
	gpio_configure_pin(SPI_MOSI_GPIO,SPI_MOSI_FLAGS);
	gpio_configure_pin(SPI_SPCK_GPIO,SPI_SPCK_FLAGS);
	gpio_configure_pin(SPI_NPCS0_GPIO,SPI_NPCS0_FLAGS);
	
	spi_enable_clock(SPI_MASTER_BASE);
	spi_disable(SPI_MASTER_BASE);
	spi_reset(SPI_MASTER_BASE);
	spi_set_lastxfer(SPI_MASTER_BASE);
	spi_set_master_mode(SPI_MASTER_BASE);
	spi_disable_mode_fault_detect(SPI_MASTER_BASE);
	spi_set_peripheral_chip_select_value(SPI_MASTER_BASE, 0);
	spi_configure_cs_behavior(SPI, 1, SPI_CS_RISE_NO_TX);
	spi_set_clock_polarity(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI_MASTER_BASE, SPI_CHIP_SEL,
	SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(SPI_MASTER_BASE, SPI_CHIP_SEL,(sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_DLYBS,SPI_DLYBCT);
	spi_enable(SPI_MASTER_BASE);
}