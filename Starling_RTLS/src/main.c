// main.c
#include <asf.h>
#include "conf_clock.h"
#include "conf_board.h"
#include "conf_usb.h"
#include "conf_spi_master.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	SystemInit();
	board_init();
	sysclk_init();

	spi_master_init(SPI);
	struct spi_device spi_dev;
	spi_dev.id = 0; //Chip Select 0

	uint32_t spi_baud_rate = 1000000; // 1Mhz for now?
	spi_master_setup_device(SPI, &spi_dev,SPI_MODE_0, spi_baud_rate, 0);
	//SPI_MODE 0 is CPOL=0, CPHA=0
	// the last argument is unused by spi_master_setup_device

	// For USB callbacks
	irq_initialize_vectors();
	cpu_irq_enable();

	udc_start();
	
	/* Insert application code here, after the board has been initialized. */
	while(1) {
		ioport_set_pin_level(LEDY, false);
		ioport_set_pin_level(LEDR, true);
		delay_ms(250);
		ioport_set_pin_level(LEDY, true);
		ioport_set_pin_level(LEDR, false);
		delay_ms(250);
		ioport_toggle_pin_level(LEDG);
		delay_ms(250);
	}

	// Based on looking at the sample SPI code and documentation, 
	// I believe this is all we need to do to write SPI
	/*
	spi_select_device(SPI, &spi_dev);
	const unsigned char* msg = "ABC";
	spi_write_packet(SPI, msg, 3);
	spi_deselect_device(SPI, &spi_dev);
	delay_ms(500);
	*/

	// We read SPI with spi_read_packet(SPI, buffer, len);
}
