// conf_usb.h
#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "compiler.h"

/**
 * USB Device Configuration
 * @{
 */

//! Device definition (mandatory)
#define  USB_DEVICE_VENDOR_ID             USB_VID_ATMEL
#define  USB_DEVICE_PRODUCT_ID            USB_PID_ATMEL_ASF_HIDGENERIC
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         0
#define  USB_DEVICE_POWER                 100 // Consumption on Vbus line (mA)
#define  USB_DEVICE_ATTR                  USB_CONFIG_ATTR_BUS_POWERED
// this flag was previously ORed with USB_DEVICE_ATTR: USB_CONFIG_ATTR_REMOTE_WAKEUP

//! USB Device string definitions (Optional)
#define  USB_DEVICE_MANUFACTURE_NAME      "AaronRay"
#define  USB_DEVICE_PRODUCT_NAME          "Starling RTLS"
// #define  USB_DEVICE_SERIAL_NAME           "12...EF"



// #define  UDC_VBUS_EVENT(b_vbus_high)      user_callback_vbus_action(b_vbus_high)
// extern void user_callback_vbus_action(bool b_vbus_high);

// start of frame event
// #define  UDC_SOF_EVENT()                  user_callback_sof_action()
// extern void user_callback_sof_action(void);

// When the host stops sending packets (including SOF packets) to the microcontroller, 
// the micro needs to go into suspend "low power" mode, drawing *very* little power.
// I think we are just going to ignore the spec on this one.

// Suspend Event
// #define  UDC_SUSPEND_EVENT()              user_callback_suspend_action()
// extern void user_callback_suspend_action(void);

// Resume Event
// #define  UDC_RESUME_EVENT()               user_callback_resume_action()
// extern void user_callback_resume_action(void);

// Don't know what REMOTE WAKEUP does, but I don't think we need it (?)

//! Mandatory when USB_DEVICE_ATTR authorizes remote wakeup feature
// #define  UDC_REMOTEWAKEUP_ENABLE()        user_callback_remotewakeup_enable()
// extern void user_callback_remotewakeup_enable(void);


// #define  UDC_REMOTEWAKEUP_DISABLE()       user_callback_remotewakeup_disable()
// extern void user_callback_remotewakeup_disable(void);



// Configure HID Callbacks
#define UDI_HID_GENERIC_ENABLE_EXT() starling_cb_generic_enable()
#define UDI_HID_GENERIC_DISABLE_EXT() starling_cb_generic_disable()
#define  UDI_HID_GENERIC_REPORT_OUT(ptr) starling_cb_generic_report_out(ptr)
#define  UDI_HID_GENERIC_SET_FEATURE(f) starling_cb_set_feature(f)

//! Sizes of I/O reports
#define  UDI_HID_REPORT_IN_SIZE             8
#define  UDI_HID_REPORT_OUT_SIZE            8
#define  UDI_HID_REPORT_FEATURE_SIZE        4

//! Sizes of I/O endpoints
#define  UDI_HID_GENERIC_EP_SIZE            64
//@}
//@}


//! The includes of classes and other headers must be done at the end of this file to avoid compile error
#include "udi_hid_generic_conf.h"
#include "starling_usb_callbacks.h"

#endif // _CONF_USB_H_
