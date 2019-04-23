#include "spi_starling.h"
#include "pio.h"
#include "spi.h"
#include "sysclk.h"
#include "asf.h"
#include "conf_board.h"

// commented out values are in microseconds, and match what you would expect for ~2MHz SPI
// these values (or faster) should be used when we get sub u-second sleeps
#define DELAYUS_SS_TO_MOSI_SET 1 //0.250
#define DELAYUS_MOSI_SET_TO_MISO_READ  1 // 0.200
#define DELAYUS_CLK_HALF_PRD 1 // 0.250
#define DELAYUS_CLK_DOWN_TO_SS_UP 1 // 2.5
#define DELAYUS_BTW_BYTE 4

#define SPI_PIN_HIGH(x) 	do { ioport_set_pin_level(x,true); asm("nop"); asm("nop"); asm("nop");} while(0);
#define SPI_PIN_LOW(x) 		do { ioport_set_pin_level(x,false); asm("nop"); asm("nop"); asm("nop");} while(0);
#define NOP_5				do { asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");} while(0);

uint8_t spi_write_byte(uint8_t data){
	uint8_t msg_rcv = 0;
	uint8_t bits[8];
	bits[7] = data & 0x01;
	bits[6] = data & 0x02;
	bits[5] = data & 0x04;
	bits[4] = data & 0x08;
	bits[3] = data & 0x10;
	bits[2] = data & 0x20;
	bits[1] = data & 0x40;
	bits[0] = data & 0x80;

	uint8_t bit_ix;
	for(bit_ix = 0; bit_ix < 8; bit_ix++){
		ioport_set_pin_level(SPI_MOSI,bits[bit_ix]);
		//delay_us(DELAYUS_CLK_HALF_PRD);
// 		asm("nop");
// 		asm("nop");
// 		asm("nop");
// 		asm("nop");
		NOP_5
		NOP_5
		NOP_5

		// read MISO on posedge clk
		msg_rcv = msg_rcv << 1;
		if (ioport_get_pin_level(SPI_MISO)){
			msg_rcv += 1;
		}
		ioport_set_pin_level(SPI_CLK,true);
		//delay_us(DELAYUS_CLK_HALF_PRD);
// 		asm("nop");
// 		asm("nop");
// 		asm("nop");
// 		asm("nop");
		NOP_5
		NOP_5
		NOP_5

		ioport_set_pin_level(SPI_CLK, false);
	}

	return msg_rcv;
}
void spi_write_msg(const void* recv_buf, const void * const send_buf, uint8_t n_bytes){
	uint8_t byte_ix;
	uint8_t msg_recv;
	ioport_set_pin_level(SPI_SEL,false);
	ioport_set_pin_level(SPI_MOSI,true);
	
	//delay_ms(DELAYUS_SS_TO_MOSI_SET);
	NOP_5
	NOP_5
	NOP_5
	for (byte_ix = 0; byte_ix < n_bytes; byte_ix++){
		msg_recv = spi_write_byte(((uint8_t *) send_buf)[byte_ix]);
		ioport_set_pin_level(SPI_MOSI,true);
		((uint8_t *) recv_buf)[byte_ix] = msg_recv;
		//delay_us(DELAYUS_BTW_BYTE);
		NOP_5
		NOP_5
		NOP_5
	}
	//delay_us(DELAYUS_CLK_DOWN_TO_SS_UP);
	NOP_5
	NOP_5
	NOP_5
	ioport_set_pin_level(SPI_SEL, true);
}
