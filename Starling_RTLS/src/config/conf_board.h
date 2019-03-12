/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define LEDY IOPORT_CREATE_PIN(PIOA, 5)
#define LEDR IOPORT_CREATE_PIN(PIOA, 2)
#define LEDG IOPORT_CREATE_PIN(PIOA, 1)

// #define SPI_MISO_GPIO  (PIO_PA12_IDX)
// #define SPI_MISO_FLAGS (PIO_PERIPH_A | PIO_DEFAULT)
// #define SPI_MOSI_GPIO  (PIO_PA13_IDX)
// #define SPI_MOSI_FLAGS (PIO_PERIPH_A | PIO_DEFAULT)
// #define SPI_SPCK_GPIO  (PIO_PA14_IDX)
// #define SPI_SPCK_FLAGS (PIO_PERIPH_A | PIO_DEFAULT)
// #define SPI_NPCS0_GPIO            (PIO_PA11_IDX)
// #define SPI_NPCS0_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)

#endif // CONF_BOARD_H
