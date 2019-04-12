// spi_comms.h -- SPI Communication
#include <stdint.h>
#include "deca_structs.h"

void send_message_delay(struct system_control *ctrl,
  struct dx_time *delay_time, int reenb_transmitter);

void set_pll(void);

void wait_for_msg(struct system_control *ctrl);

void wait_for_msg_int(
  struct system_control *ctrl,
  struct system_status *sta);

void clear_status(struct system_status *sta);

uint64_t time_add(uint64_t curr_time, uint64_t delay_time);

uint64_t time_sub(uint64_t t_larger, uint64_t t_smaller);

int get_rx_data(struct rx_data *data);

int write_payload(struct mac_header * const mac,
              const unsigned char * const payload,
              const int len,
              const uint64_t timestamp);

void reset_trx(struct system_control *ctrl);


