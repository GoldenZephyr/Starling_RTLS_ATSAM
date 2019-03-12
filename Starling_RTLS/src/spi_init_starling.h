#ifndef SPI_INIT_STARLING_H_
#define SPI_INIT_STARLING_H_

#include <stdint.h>

void spi_init(void);
uint8_t spi_rx(void);
void spi_tx(uint8_t data,uint8_t last_byte);



#endif /* SPI_INIT_STARLING_H_ */