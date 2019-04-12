#ifndef DECA_INIT_H_
#define DECA_INIT_H_

#include "deca_structs.h"
#include <stdint.h>

int spi_init(void);

int comms_check(void);

int decawave_comms_init(const uint16_t pan_id,
						const uint16_t addr_id,
						const struct tx_fctrl * const fctrl);

//Initializes tx_fctrl with our parameters
void frame_control_init(struct tx_fctrl *fctrl);

void tx_buffer_init(struct tx_buffer *tx_buff);

void sys_conf_init(struct system_conf *conf);

void sys_ctrl_init(struct system_control *ctrl);

void sys_mask_init(struct system_mask *mask);

void load_microcode(void);

#endif /* DECA_INIT_H_ */