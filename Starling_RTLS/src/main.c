// main.c
#include <asf.h>
#include "conf_clock.h"
#include "conf_board.h"
#include "conf_usb.h"
#include "spi_starling.h"
#include "deca_structs.h"
#include "ranging.h"

extern volatile bool new_usb_cmd;
extern volatile uint8_t rp_mod[UDI_HID_REPORT_OUT_SIZE];

bool i_am_sender = false;

int main(void) {

	bool do_usb = true;
	//SystemInit();
	board_init();
	sysclk_init();

	REG_WDT_MR |= WDT_MR_WDDIS;

	while(1){
		ioport_set_pin_level(TP1,true);
		asm("nop");
		asm("nop");
		ioport_set_pin_level(TP1,false);
		asm("nop");
		asm("nop");
		ioport_set_pin_level(TP1,true);
		asm("nop");
		asm("nop");
		ioport_set_pin_level(TP1,false);
		asm("nop");
		asm("nop");
		ioport_set_pin_level(TP1,true);
		asm("nop");
		asm("nop");
		ioport_set_pin_level(TP1,false);
		asm("nop");
		asm("nop");
	}

	// For USB callbacks
 	if (do_usb) {
 		irq_initialize_vectors();
 		cpu_irq_enable();
 		udc_start();
 		udc_attach();
 	}
	
	ioport_set_pin_level(LEDR, true);
	ioport_set_pin_level(LEDG, true);
	ioport_set_pin_level(LEDY, true);

	// Startup sequence
	ioport_toggle_pin_level(LEDG);
	delay_ms(1000);
	ioport_toggle_pin_level(LEDR);
	delay_ms(1000);
	ioport_toggle_pin_level(LEDY);
	delay_ms(1000);
	ioport_toggle_pin_level(LEDY);
	delay_ms(1000);
	ioport_toggle_pin_level(LEDR);
	delay_ms(1000);
	ioport_toggle_pin_level(LEDG);
	delay_ms(2000);

// 	int ix = 0;
// 	while(1){
// 		ioport_set_pin_level(LEDG, (ix & 0x1) == 0);
// 		ioport_set_pin_level(LEDR, (ix & 0x2) == 0);
// 		ioport_set_pin_level(LEDY, (ix & 0x4) == 0);
// 		delay_ms(2500);
// 		ix += 1;
// 	}

	struct system_control ctrl;
	struct system_status sta;
	struct tx_buffer tx_buff;
	struct range_info info;

	int spi_error  = spi_ranging_setup(&ctrl, &sta, &tx_buff);
	if (spi_error > 0){
		ioport_set_pin_level(LEDR, false);
		delay_ms(1000);
		ioport_set_pin_level(LEDR, true);
		}else{
		ioport_set_pin_level(LEDG, false);
		delay_ms(1000);
		ioport_set_pin_level(LEDG, true);
	}

	while(1) {

		ioport_set_pin_level(LEDR, false);
		delay_ms(1000);
		ioport_set_pin_level(LEDR, true);

		info.timestamp_rx_1 = 0;
		info.timestamp_tx_1 = 0;
		info.timestamp_rx_2 = 0;
		info.timestamp_tx_2 = 0;
		info.timestamp_rx_3 = 0;
		info.timestamp_tx_3 = 0;

		if (i_am_sender){
			ranging_send(&ctrl, &sta, &tx_buff, &info);
		} else {
			ranging_recv(&ctrl, &sta, &tx_buff, &info);
		}
		uint64_t tx1 = info.timestamp_tx_1;

		if(do_usb){
// 			if (new_usb_cmd) {
// 				udi_hid_generic_send_report_in(rp_mod);
// 				new_usb_cmd = false;
// 			}

			udi_hid_generic_send_report_in( (uint8_t *) &info);
		}

		ioport_set_pin_level(LEDY, false);
		delay_ms(1000);
		ioport_set_pin_level(LEDY, true);
	}
}
