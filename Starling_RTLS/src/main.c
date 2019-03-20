// main.c
#include <asf.h>
#include "conf_clock.h"
#include "conf_board.h"
#include "conf_usb.h"
//#include "conf_spi_master.h"
#include "spi_init_starling.h"

extern volatile bool new_usb_cmd;
extern volatile uint8_t rp_mod[UDI_HID_REPORT_OUT_SIZE];


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	bool do_usb = false;
	SystemInit();
	board_init();
	sysclk_init();
	//spi_init();


	//spi_master_init(SPI);
	//struct spi_device spi_dev;
	//spi_dev.id = 0; //Chip Select 0

	uint32_t spi_baud_rate = 2000000; // 1Mhz for now?
	//spi_master_setup_device(SPI, &spi_dev,SPI_MODE_0, spi_baud_rate, 0);
	//spi_enable(SPI);
	//SPI_MODE 0 is CPOL=0, CPHA=0
	// the last argument is unused by spi_master_setup_device

	// For USB callbacks
	if (do_usb) {
		irq_initialize_vectors();
		cpu_irq_enable();
		udc_start();
		udc_attach();
	}
	
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

		if(do_usb){
			if (new_usb_cmd) {
				udi_hid_generic_send_report_in(rp_mod);
				new_usb_cmd = false;
			}
		}

		uint8_t send_buf[5] = {0};
		uint8_t recv_buf[5] = {0};
		uint8_t n_bytes = 5;

		spi_write_msg(send_buf, recv_buf, n_bytes);
		uint8_t recv1 = recv_buf[0];
		uint8_t recv2 = recv_buf[1];
		uint8_t recv3 = recv_buf[2];
		uint8_t recv4 = recv_buf[3];
		uint8_t recv5 = recv_buf[4];


// 		spi_select_device(SPI, &spi_dev);
 		//const unsigned char* msg_cmd = "\0";
//  		uint8_t recv_buf[4];
// 		uint8_t tosend = 0;
// 		uint8_t tmp1 = 0;
// 		uint8_t tmp2 = 0;
// 		uint8_t tmp3 = 0;
// 		uint8_t tmp4 = 0;
// 		uint8_t tmp5 = 0;
// 
// 		spi_tx(tosend,0);
// 		tmp1 = spi_rx();
// 
// 
// 		spi_tx(tosend,0);
// 		tmp2 = spi_rx();
// 		recv_buf[0] = tmp2;
// 
// 		spi_tx(tosend,0);
// 		tmp3 = spi_rx();
// 		recv_buf[1] = tmp3;
// 
// 		spi_tx(tosend,0);
// 		tmp4 = spi_rx();
// 		recv_buf[2] = tmp4;
// 
// 		spi_tx(tosend,1);
// 		tmp5 = spi_rx();
// 		recv_buf[3] = tmp5;

// 		//spi_write(SPI, msg_cmd, )
// 		spi_write_packet(SPI, msg_cmd, 1);
// 		spi_read_packet(SPI, recv_buf, 4);
// 		spi_deselect_device(SPI, &spi_dev);
	}

	// Based on looking at the sample SPI code and documentation, 
	// I believe this is all we need to do to write SPI

	// We read SPI with spi_read_packet(SPI, buffer, len);
}
