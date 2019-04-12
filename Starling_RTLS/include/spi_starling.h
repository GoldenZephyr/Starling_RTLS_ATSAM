#ifndef SPI_INIT_STARLING_H_
#define SPI_INIT_STARLING_H_

#include <stdint.h>

uint8_t spi_write_byte(uint8_t data);
void spi_write_msg(const void* recv_buf, const void * const send_buf, uint8_t n_bytes);

#endif /* SPI_INIT_STARLING_H_ */