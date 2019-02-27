#ifndef STARLING_USB_CALLBACKS_H_
#define STARLING_USB_CALLBACKS_H_

#include <stdbool.h>
#include <stdint.h>
#include "udi_hid_generic.h"

bool starling_cb_generic_enable(void);

void starling_cb_generic_disable(void);

// These are querying messages from the host
void starling_cb_generic_report_out(uint8_t *report);

void starling_cb_set_feature(uint8_t *report_feature);


#endif /* STARLING_USB_CALLBACKS_H_ */