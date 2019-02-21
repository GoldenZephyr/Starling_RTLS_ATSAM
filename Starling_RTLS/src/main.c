/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include "conf_clock.h"
#include "conf_board.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	SystemInit();
	board_init();
	sysclk_init();
	
	/* Insert application code here, after the board has been initialized. */
	while(1) {
		ioport_set_pin_level(LEDY, false);
		ioport_set_pin_level(LEDR, true);
		delay_ms(500);
		ioport_set_pin_level(LEDY, true);
		ioport_set_pin_level(LEDR, false);
		delay_ms(500);
		ioport_toggle_pin_level(LEDG);
		delay_ms(500);
	}
}
