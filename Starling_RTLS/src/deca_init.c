#include "deca_init.h"
#include "deca_constants.h"
#include "deca_structs.h"
#include "spi_starling.h"
#include "sysclk.h"
#include "asf.h"
#include <string.h>


//Initializes tx_fctrl with our parameters
void frame_control_init(struct tx_fctrl *fctrl) {
	fctrl->reg      = TX_FCTRL_REG | WRITE;
	fctrl->tflen    = 0x0B + PAYLOAD_LEN; //Frame Length - MAC Header + Payload
	fctrl->tfle     = 0x00;  //Extended Frame - No
	fctrl->res_1    = 0x00;  //Reserved Bits - Write 0
	fctrl->txbr     = 0x02;  //Transmit Bitrate - 6.8Mbps
	fctrl->tr       = 0x00;  //Ranging Frame - No (Unused)
	fctrl->txprf    = 0x01;  //Transmit Preamble Repitition Rate - 16Mhz
	fctrl->txpsr    = 0x01;
	fctrl->pe       = 0x01;  //Preamble Length Selection - 128 Symbols
	fctrl->txbodds  = 0x00;  //Transmit Buffer Offset - 0 Bytes
	fctrl->ifsdelay = 0x00;  //Minimum Time Between Frame Sends - 0 Symbols
	fctrl->res_2    = 0x00;  //Reserved Bits - Write 0
}

void tx_buffer_init(struct tx_buffer *tx_buff) {
	tx_buff->reg = TX_BUFFER_REG | WRITE;
	struct mac_header *mac = &tx_buff->mac_header;
	struct frame_control *fcs = &mac->frame_control;

	//Setup Frame_Contol
	fcs->frame_type = 0x01; //Frame Type - Data
	fcs->security_enabled = 0x00; //Security Enabled - Nah man
	fcs->frame_pending = 0x00; //More Data Incoming - No
	fcs->ack_request = 0x00; //Request Acknowledge Message - No
	fcs->pan_id_compress = 0x01; //PAN ID Compression - No
	fcs->reserved = 0x00; //Reserved Bits - Write 0
	fcs->dest_addr_mode = 0x02; //Destination Address Mode - Short (16-bit)
	fcs->frame_version = 0x01; //Frame Version - IEEE 802.15.4 TODO: ???
	fcs->source_address_mode = 0x02; //Source Addressing Mode - Short (16-bit)

	//Setup MAC Header
	mac->sequence_number = 0x01; //Sequence # TODO: ???
	mac->dest_pan_id = PAN_ID_LO | (PAN_ID_HI << 8);
	mac->dest_addr = PAN_ID_LO | (PAN_ID_HI << 8);
	mac->source_pan_id = PAN_ID_LO | (PAN_ID_HI << 8);
	mac->source_addr = ADDR_ID_LO | (ADDR_ID_HI << 8);
}


void sys_ctrl_init(struct system_control *ctrl) {
	ctrl->reg = SYS_CTRL_REG | WRITE;
	ctrl->sfcst = 0; //Auto append FCS - Default No
	ctrl->txstrt = 0; //Start Transmittion - Default No
	ctrl->txdlys = 0; //Transmission Delay - Default No
	ctrl->cansfcs = 0; //High Speed Transmission - Default No
	ctrl->res = 0; //Reserved Bits - Write 0
	ctrl->trxoff = 0; //Disable Transeiver - Default No
	ctrl->waitresp = 0; //Enable Receiver after sending - Default No
	ctrl->rxenab = 0; //Enable Receiver - Default No
	ctrl->rxdlye = 0; //Receive Delay - Default No
	ctrl->res_2 = 0; //Reserved Bits - Write 0
	ctrl->hrbpt = 0; //Double Buffer Mode - Default 0
	ctrl->res_3 = 0; //Reserved Bits - Write 0
}

void sys_conf_init(struct system_conf *conf) {
	conf->reg = SYS_CONF_REG | WRITE;
	conf->ffen = 0; //Frame filtering enable - no
	conf->ffbc = 1; //FF as Coordinator - yes
	conf->ffab = 1; //FF Allow Beacon - yes
	conf->ffad = 1; //FF Allow Data - yes
	conf->ffaa = 1; //FF Allow Ack - yes
	conf->ffam = 1; //FF Allow MAC Command - yes
	conf->ffar = 1; //FF Allow Reserved - yes
	conf->ffaf = 1; //FF Allow Frames with type field 4 - Yes
	conf->ffav = 1; //FF Allow " " " " 5 - Yes
	conf->hirq = 1; //Use Interrupt GPIO - Yes - Active High
	conf->spiedge = 0; //SPI Edge mode - Default
	conf->disfce = 0; //Disable error handling - No
	conf->disdrxb = 1; //Disable Double Buffer - Yes
	conf->disphe = 0; //Disable on PHR error - Yes
	conf->disrsde = 0; //Disable receiver on RSD Error - yes
	conf->fcsinit = 0; //FCS Seed mode - Default
	conf->phrmode = 0x00; //PHR - Standard Frame
	conf->disstxp = 0; //Smart TX Power - Default
	conf->res = 0; //Reserved - Write 0
	conf->rxmk = 0; //110kbps mode - No
	conf->res_2 = 0; //Reserved - Write 0
	conf->rxwtoe = 0; //Timeout Receiver - No
	conf->rxautr = 1; //Auto-reenable receive - Yes
	conf->autoack = 0; //Auto-Ackknowledge - No
	conf->aackpend = 0; //Auto-Append - No
}

//Interrupt GPIO Mask (Masked = Will show)
void sys_mask_init(struct system_mask *mask) {
	memset(mask, 0x00, sizeof(struct system_mask));
	mask->reg = SYS_MASK_REG | WRITE;
	mask->mrxdfr = 0x01; //Mask Data Frame Ready
	char mask_rx[SYS_MASK_LEN];
	spi_write_msg(mask_rx, mask, SYS_MASK_LEN);
}

void load_microcode(void) {
	unsigned char tx_temp[4] = {0x00};
	unsigned char rx_temp[4] = {0x00};
	tx_temp[0] = 0x36 | WRITE | SUB_INDEX;
	tx_temp[1] = 0x00; //Sub Index 0x00
	tx_temp[2] = 0x01;
	tx_temp[3] = 0x03;
	spi_write_msg(rx_temp, tx_temp, 4);
	tx_temp[0] = 0x2D | WRITE | SUB_INDEX;
	tx_temp[1] = 0x06;
	tx_temp[2] = 0x00;
	tx_temp[3] = 0x80;
	spi_write_msg(rx_temp, tx_temp, 4);
	delay_us(200);
	tx_temp[0] = 0x36 | WRITE | SUB_INDEX;
	tx_temp[1] = 0x00; //Sub Index 0x00
	tx_temp[2] = 0x00;
	tx_temp[3] = 0x02;
	spi_write_msg(rx_temp, tx_temp, 4);
  }

  //Initialize Communications
int decawave_comms_init(const uint16_t pan_id,
                        const uint16_t addr_id,
						const struct tx_fctrl * const fctrl) {
	//Write the PAN ID and Addr ID
    struct panaddr pan_msg;
    pan_msg.reg = PANADDR_REG | WRITE;
    pan_msg.pan_id = pan_id;
    pan_msg.addr_id = addr_id;
    unsigned char rx_buf[PANADDR_LEN] = {0x00};
    spi_write_msg(rx_buf, &pan_msg, PANADDR_LEN);
    // Write Check - Unit Test
    char tx_buf[PANADDR_LEN] = {PANADDR_REG, 0x00, 0x00, 0x00, 0x00};
    spi_write_msg(rx_buf, tx_buf, PANADDR_LEN);
    //debug_print("0x%02X 0x%02X 0x%02X 0x%02X\n",
    //       rx_buf[1], rx_buf[2], rx_buf[3], rx_buf[4]);
    if ((rx_buf[1] | (rx_buf[2] << 8)) != pan_id) {
      //debug_print("Did not set PAN_ID (reading 0x%02X 0x%02X)\n", rx_buf[1], rx_buf[2]);
      return 1;
    }
    if ((rx_buf[3] | (rx_buf[4] << 8)) != addr_id) {
      //debug_print("Did not set ADDR_ID (reading 0x%02X 0x%02X)\n", rx_buf[1], rx_buf[2]);
      return 1;
    }

    //Write the tx_fctrl register
    unsigned char rx_fctrl_buf[TX_FCTRL_LEN] = {0x00};
    spi_write_msg(rx_fctrl_buf, fctrl, TX_FCTRL_LEN);
    return 0;
  }