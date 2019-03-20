/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	ioport_init();
	ioport_set_pin_dir(LEDY, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LEDG, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LEDR, IOPORT_DIR_OUTPUT);

	ioport_set_pin_dir(SPI_MOSI, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SPI_CLK, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SPI_MISO, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SPI_SEL, IOPORT_DIR_OUTPUT);

	ioport_set_pin_dir(TP1, IOPORT_DIR_OUTPUT);


// 	gpio_configure_pin(SPI_MISO_GPIO,SPI_MISO_FLAGS);
// 	gpio_configure_pin(SPI_MOSI_GPIO,SPI_MOSI_FLAGS);
// 	gpio_configure_pin(SPI_SPCK_GPIO,SPI_SPCK_FLAGS);
// 	gpio_configure_pin(SPI_NPCS0_GPIO,SPI_NPCS0_FLAGS);
}
