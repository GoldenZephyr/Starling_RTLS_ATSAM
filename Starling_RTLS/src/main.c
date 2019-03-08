// main.c
#include <asf.h>
#include "conf_clock.h"
#include "conf_board.h"
#include "conf_usb.h"
#include "conf_spi_master.h"

extern volatile bool new_usb_cmd;
extern volatile uint8_t rp_mod[UDI_HID_REPORT_OUT_SIZE];

//Paramteres:
//  data: data to be sent
//  last_byte: either 0 or 1. 
//   0-There are following bytes to be sent. The following bytes and the current byte composes one data-unit, CS should be kept low after sending out the current byte
//   1-Last byte of a data unit. CS will be set to HIGH when the current byte has been sent out.
void spi_tx(uint8_t data,uint8_t last_byte)
{
 spi_write(SPI,data,0,last_byte);
 while ((spi_read_status(SPI) & SPI_SR_RDRF) == 0);/* Wait transfer done. */
}
 
uint8_t spi_rx(void)
{
 uint8_t tmp;
 spi_read (SPI,&tmp,0);
 return tmp;
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	bool do_usb = false;
	SystemInit();
	board_init();
	sysclk_init();

	spi_master_init(SPI);
	struct spi_device spi_dev;
	spi_dev.id = 0; //Chip Select 0

	uint32_t spi_baud_rate = 2000000; // 1Mhz for now?
	spi_master_setup_device(SPI, &spi_dev,SPI_MODE_0, spi_baud_rate, 0);
	spi_enable(SPI);
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

		spi_select_device(SPI, &spi_dev);
		const unsigned char* msg_cmd = "\0";
		unsigned char recv_buf[4];
		//spi_write(SPI, msg_cmd, )
		spi_write_packet(SPI, msg_cmd, 1);
		spi_read_packet(SPI, recv_buf, 4);
		spi_deselect_device(SPI, &spi_dev);
		//delay_ms(1000);
	}

	// Based on looking at the sample SPI code and documentation, 
	// I believe this is all we need to do to write SPI

	// We read SPI with spi_read_packet(SPI, buffer, len);
}
