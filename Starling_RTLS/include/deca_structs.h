#ifndef DECA_STRUCTS_H_
#define DECA_STRUCTS_H_

#include "deca_constants.h"

struct __attribute__((__packed__)) panaddr {
	uint8_t reg;
	uint16_t pan_id;
	uint16_t addr_id;
};

//TX Fctrl - 9 Bytes
struct __attribute__((packed)) tx_fctrl {
	unsigned int reg : 8;
	unsigned int tflen : 7;
	unsigned int tfle : 3;
	unsigned int res_1 : 3;
	unsigned int txbr : 2;
	unsigned int tr : 1;
	unsigned int txprf : 2;
	unsigned int txpsr : 2;
	unsigned int pe : 2;
	unsigned int txbodds : 10;
	unsigned int ifsdelay : 8;
	unsigned int res_2 : 24;
};

struct __attribute__((__packed__)) rx_fqual {
	uint8_t reg;
	uint16_t std_noise;
	uint16_t fp_ampl2;
	uint16_t pp_ampl3;
	uint16_t cir_pwr;
};
//Frame Control - 2 Bytes
struct __attribute__((__packed__)) frame_control {
	unsigned int frame_type : 3;
	unsigned int security_enabled : 1;
	unsigned int frame_pending : 1;
	unsigned int ack_request : 1;
	unsigned int pan_id_compress : 1;
	unsigned int reserved : 3;
	unsigned int dest_addr_mode : 2;
	unsigned int frame_version : 2;
	unsigned int source_address_mode : 2;
};

//MAC Header - 11 Bytes
struct __attribute__((__packed__)) mac_header {
	struct frame_control frame_control;
	uint8_t sequence_number;
	uint16_t dest_pan_id;
	uint16_t dest_addr;
	uint16_t source_pan_id;
	uint16_t source_addr;
};

//TX Buffer -  Octets
struct __attribute__((__packed__)) tx_buffer {
	uint8_t reg;
	struct mac_header mac_header;
	uint64_t timestamp_tx : 40;
	uint16_t fp_ampl1;
	uint16_t rxpacc_rx;
	struct rx_fqual fqual;
	uint8_t payload[PAYLOAD_LEN]; //TODO: Longer? 64 Bytes
};

//System Control - 4 Bytes
struct __attribute__((__packed__)) system_control {
	unsigned int reg : 8;
	unsigned int sfcst : 1;
	unsigned int txstrt : 1;
	unsigned int txdlys : 1;
	unsigned int cansfcs : 1;
	unsigned int res : 2;
	unsigned int trxoff : 1;
	unsigned int waitresp : 1;
	unsigned int rxenab : 1;
	unsigned int rxdlye : 1;
	unsigned int res_2 : 14;
	unsigned int hrbpt : 1;
	unsigned int res_3 : 7;
};

//System Configure - 4 Bytes
struct __attribute__((__packed__)) system_conf {
	unsigned int reg : 8;
	unsigned int ffen : 1;
	unsigned int ffbc : 1;
	unsigned int ffab : 1;
	unsigned int ffad : 1;
	unsigned int ffaa : 1;
	unsigned int ffam : 1;
	unsigned int ffar : 1;
	unsigned int ffaf : 1;
	unsigned int ffav : 1;
	unsigned int hirq : 1;
	unsigned int spiedge : 1;
	unsigned int disfce : 1;
	unsigned int disdrxb : 1;
	unsigned int disphe : 1;
	unsigned int disrsde : 1;
	unsigned int fcsinit : 1;
	unsigned int phrmode : 2;
	unsigned int disstxp : 1;
	unsigned int res : 3;
	unsigned int rxmk : 1;
	unsigned int res_2 : 5;
	unsigned int rxwtoe : 1;
	unsigned int rxautr : 1;
	unsigned int autoack : 1;
	unsigned int aackpend : 1;
};


//System Event Mask - 4 Bytes
struct __attribute__((__packed__)) system_mask {
	unsigned int reg : 8;
	unsigned int res : 1;
	unsigned int mcplock : 1;
	unsigned int mesyncr: 1;
	unsigned int maat : 1;
	unsigned int mtxfrb : 1;
	unsigned int mtxprs : 1;
	unsigned int mtxphs : 1;
	unsigned int mtxfrs : 1;
	unsigned int mrxprd : 1;
	unsigned int mrxsfdd : 1;
	unsigned int mldedone : 1;
	unsigned int mrxphd : 1;
	unsigned int mrxphe : 1;
	unsigned int mrxdfr : 1;
	unsigned int mrxfcg : 1;
	unsigned int mrxfce : 1;
	unsigned int mrxrfsl : 1;
	unsigned int mrxrtfo : 1;
	unsigned int mldeerr : 1;
	unsigned int res_2 : 1;
	unsigned int mrxovrr : 1;
	unsigned int mrxpto : 1;
	unsigned int mgpioirq : 1;
	unsigned int mslp2ini : 1;
	unsigned int mrfpllll : 1;
	unsigned int mcpllll : 1;
	unsigned int mrxsfdto : 1;
	unsigned int mhpdwarn : 1;
	unsigned int mtxberr : 1;
	unsigned int maffrej : 1;
	unsigned int res_3 : 2;
};

//System Status - 5 Bytes
struct __attribute__((__packed__)) system_status {
	unsigned int reg : 8;
	unsigned int irqs : 1;
	unsigned int cplock : 1;
	unsigned int esyncr : 1;
	unsigned int aat : 1;
	unsigned int txfrb : 1;
	unsigned int txprs : 1;
	unsigned int txphs : 1;
	unsigned int txfrs : 1;
	unsigned int rxprd : 1;
	unsigned int rxsfdd : 1;
	unsigned int ldedone : 1;
	unsigned int rxphd : 1;
	unsigned int rxphe : 1;
	unsigned int rxdfr : 1;
	unsigned int rxfcg : 1;
	unsigned int rxfce : 1;
	unsigned int rxrfsl : 1;
	unsigned int rxrfto : 1;
	unsigned int ldeerr : 1;
	unsigned int res : 1;
	unsigned int rxovrr : 1;
	unsigned int rxpto : 1;
	unsigned int gpioirq : 1;
	unsigned int slpinit : 1;
	unsigned int rfpll : 1;
	unsigned int clkpll : 1;
	unsigned int rxsfdto : 1;
	unsigned int hpdwarn : 1;
	unsigned int txberr : 1;
	unsigned int affrej : 1;
	unsigned int hsrbp : 1;
	unsigned int icrbp : 1;
	unsigned int rxrscs : 1;
	unsigned int rxprej : 1;
	unsigned int txpute : 1;
	unsigned int res_2 : 5;
};

//RX Info - 4 Bytes
struct __attribute__((__packed__)) rx_finfo {
	unsigned int reg : 8;
	unsigned int rfxlen : 7;
	unsigned int rxfle : 3;
	unsigned int res : 1;
	unsigned int rxnspl : 2;
	unsigned int rxbr : 2;
	unsigned int rng : 1;
	unsigned int rxprfr : 2;
	unsigned int rxprsr : 2;
	unsigned int rxpacc : 12;
};

struct __attribute__((__packed__)) rx_time {
	uint64_t reg : 8;
	uint64_t rx_stamp : 40;
	uint64_t fp_index : 16;
	uint64_t fp_ampl1 : 16;
	uint64_t rx_rawst : 40;
};


struct __attribute__((__packed__)) rx_buffer { //Yeah it's tx_buffer...
	uint8_t reg;
	struct mac_header mac_header;
	uint64_t timestamp_tx : 40;
	uint16_t fp_ampl1;
	uint16_t rxpacc_rx;
	struct rx_fqual fqual;
	uint8_t payload[PAYLOAD_LEN];
};



//Wrapper Struct - Has all transmission RX information
struct __attribute__((__packed__)) rx_data {
	uint8_t reg;
	struct rx_finfo finfo;
	struct rx_time timestamp;
	struct rx_buffer buffer;
	struct rx_fqual fqual;
};

struct __attribute__((__packed__)) dx_time {
	uint8_t reg;
	uint64_t delay_time : 40;
};

struct __attribute__((__packed__)) sys_time {
	uint8_t reg;
	uint64_t curr_time : 40;
};

struct __attribute__((__packed__)) range_info {
	uint64_t timestamp_tx_1 : 40;
	uint64_t timestamp_rx_1 : 40;
	uint64_t timestamp_tx_2 : 40;
	uint64_t timestamp_rx_2 : 40;
	uint64_t timestamp_tx_3 : 40;
	uint64_t timestamp_rx_3 : 40;
	uint16_t fp_ampl1_rx_1;
	uint16_t fp_ampl1_rx_2;
	uint16_t fp_ampl1_rx_3;
	uint16_t rxpacc_rx_1;
	uint16_t rxpacc_rx_2;
	uint16_t rxpacc_rx_3;
	struct rx_fqual rx_fqual_1;
	struct rx_fqual rx_fqual_2;
	struct rx_fqual rx_fqual_3;
};

struct __attribute__((__packed__)) ec_ctrl {
	uint8_t reg;
	unsigned int ostsm : 1;
	unsigned int osrsm : 1;
	unsigned int pllldt : 1;
	unsigned int wait : 8;
	unsigned int ostrm : 1;
	unsigned int res : 20;
};



#endif /* DECA_STRUCTS_H_ */