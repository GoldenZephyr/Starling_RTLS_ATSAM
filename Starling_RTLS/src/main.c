// main.c
#include <asf.h>
#include "conf_clock.h"
#include "conf_board.h"
#include "conf_usb.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	SystemInit();
	board_init();
	sysclk_init();

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
}
