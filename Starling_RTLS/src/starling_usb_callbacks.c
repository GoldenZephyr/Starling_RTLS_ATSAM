// starling_usb_callbacks.c
#include "starling_usb_callbacks.h"

volatile bool generic_hid_enable = false;
volatile bool new_usb_cmd = false;
volatile uint8_t rp_mod[UDI_HID_REPORT_OUT_SIZE];

bool starling_cb_generic_enable(void){
	generic_hid_enable = true;
	return true;
}

void starling_cb_generic_disable(void){
	generic_hid_enable = false;
}

// Queries from master
void starling_cb_generic_report_out(uint8_t *report){
	rp_mod[0] = ++report[0];
	new_usb_cmd = true;
	//udi_hid_generic_send_report_in(rp_mod);
}

// This is called when there is a "set feature" command
// Which apparently is used for things like sendings settings or shutdown commands
void starling_cb_set_feature(uint8_t *report_feature){

}