#ifndef RANGING_H_
#define RANGING_H_

#include "deca_structs.h"

// Sets up the decawave and some control structures to be ready for ranging
int spi_ranging_setup(struct system_control *ctrl,
					  struct system_status *sta,
					  struct tx_buffer *tx_buff);

// Puts the sensor in the "Ranging Initiator" (Node A, sender) state machine
void ranging_send(
	struct system_control *ctrl,
	struct system_status *sta,
	struct tx_buffer *tx_buff,
	struct range_info *info);

// Puts the sensor in the "Ranging Responder" (Node B, receiver) state machine
void ranging_recv(
	struct system_control *ctrl,
	struct system_status *sta,
	struct tx_buffer *tx_buff,
	struct range_info *info);

#endif /* RANGING_H_ */