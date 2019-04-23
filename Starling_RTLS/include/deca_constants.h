#ifndef DECA_CONSTANTS_H_
#define DECA_CONSTANTS_H_

#include "deca_structs.h"
#include <stdint.h>

#define WRITE 0x80
#define SUB_INDEX 0x40

#define DEV_ID_REG 0x00
#define DEV_ID_LEN 5

#define SYS_TIME_REG 0x06
#define SYS_TIME_LEN 6

#define PANADDR_REG 0x03
#define PANADDR_LEN 5

#define SYS_CONF_REG 0x04
#define SYS_CONF_LEN 5

#define PAN_ID_LO 0xFF
#define PAN_ID_HI 0xFF
#define ADDR_ID_LO 0xFF
#define ADDR_ID_HI 0xFF

#define TX_FCTRL_REG 0x08
#define TX_FCTRL_LEN 6

#define PAYLOAD_LEN 64

#define TX_BUFFER_REG 0x09
#define TX_BUFFER_LEN ((uint8_t) sizeof(struct tx_buffer))

#define RX_BUFFER_REG 0x11
#define RX_BUFFER_LEN (sizeof(struct rx_buffer))

#define RX_FINFO_REG 0x10
#define RX_FINFO_LEN 5

#define RX_TIME_REG 0x15
#define RX_TIME_LEN 15

#define SYS_CTRL_REG 0x0D
#define SYS_CTRL_LEN 5

#define SYS_MASK_REG 0x0E
#define SYS_MASK_LEN 5

#define SYS_STATUS_REG 0x0F
#define SYS_STATUS_LEN 6

#define DX_TIME_REG 0x0A
#define DX_TIME_LEN 6

#define SYS_TIME_REG 0x06
#define SYS_TIME_LEN 6

#define EC_CTRL_REG 0x24
#define EC_CTRL_LEN 5

#define RX_FQUAL_REG 0x12
#define RX_FQUAL_LEN 9

#define LIGHT_SPEED 299792458
#define CLOCK_FREQ ((uint64_t) 128*499200000)
#define T_REPLY ((uint64_t) CLOCK_FREQ/100) //10 ms

#endif /* DECA_CONSTANTS_H_ */