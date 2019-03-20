#ifndef SPI_INIT_STARLING_H_
#define SPI_INIT_STARLING_H_

#include <stdint.h>

//void spi_init(void);
//uint8_t spi_rx(void);
//void spi_tx(uint8_t data,uint8_t last_byte);

uint8_t spi_write_byte(uint8_t data);
void spi_write_msg(uint8_t* send_buf, uint8_t* recv_buf, uint8_t n_bytes);



#endif /* SPI_INIT_STARLING_H_ */