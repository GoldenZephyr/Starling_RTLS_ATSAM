#include "ranging.h"
#include "spi_comms.h"
#include "deca_constants.h"
#include "deca_structs.h"
#include "deca_init.h"
#include "spi_starling.h"
#include <string.h>

int spi_ranging_setup(struct system_control *sys_ctrl,
					  struct system_status *sta,
					  struct tx_buffer *tx_buff){

	int comms_ok = comms_check();
	if (comms_ok == 0){
		return 1;
	}
	sys_ctrl_init(sys_ctrl);
	set_pll();

	//Check System Status
	char tx_status[SYS_STATUS_LEN] = {0x00};
	char rx_status[SYS_STATUS_LEN] = {0x00};
	tx_status[0] = SYS_STATUS_REG;
	spi_write_msg(sta, tx_status, SYS_STATUS_LEN);
	memcpy(rx_status, sta, sizeof(struct system_status));
	//printf("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", rx_status[1],
	//rx_status[2], rx_status[3], rx_status[4], rx_status[5]);
	clear_status(sta);
	spi_write_msg(sta, tx_status, SYS_STATUS_LEN);
	memcpy(rx_status, sta, sizeof(struct system_status));
	//printf("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", rx_status[1],
	//rx_status[2], rx_status[3], rx_status[4], rx_status[5]);

	//Setup tx_fctrl
	struct tx_fctrl fctrl;
	frame_control_init(&fctrl);

	tx_buffer_init(tx_buff);

	if (decawave_comms_init(0xFFFF, 0xFFFF, &fctrl) == 1) {
		//this is bad
		asm("nop"); // so we can put breakpoint here easily
		return 1;
	}
	load_microcode();
	reset_trx(sys_ctrl);

	return 0;
}

void ranging_send(struct system_control *ctrl,
	struct system_status *sta, struct tx_buffer *tx_buff, 
    struct range_info *info) {
    
    //Get current time
    struct sys_time curr_time = {0x00};
    unsigned char tx_sys_time[SYS_TIME_LEN] = {0x00};
    tx_sys_time[0] = SYS_TIME_REG;
    spi_write_msg(&curr_time, tx_sys_time, SYS_TIME_LEN);
    
    //Determine intial sending time
    uint64_t timestamp_tx_1 = time_add(curr_time.curr_time, T_REPLY);
    
    //Put timestamp into delayed sending buffer & payload structs
    struct dx_time delay_time;  
    delay_time.reg = DX_TIME_REG | WRITE;
    delay_time.delay_time = timestamp_tx_1;
    tx_buff->timestamp_tx = timestamp_tx_1;
    
    //SPI Write in new buffer
    char buff_rx[TX_BUFFER_LEN];
    spi_write_msg(buff_rx, tx_buff, TX_BUFFER_LEN);
    //debug_print("Ok, I will intialize the ranging at time %llu\n", timestamp_tx_1);
    
    //Send Delayed Message Command
    send_message_delay(ctrl, &delay_time, 1);
    
    //Go into recieve mode - Wait for Message
    //debug_print("Sent message, waiting for reply...\n");
    wait_for_msg_int(ctrl, sta);
    //We got a reply!
    
    //Grab RX Info
    //debug_print("Got message, grabbing data...\n");
    struct rx_data data;
    get_rx_data(&data);
    
    //RX Timestamp (When we got it's reply)
    uint64_t timestamp_rx_2 = data.timestamp.rx_stamp; //Lower 40 Bits
    
    //TX Timestamp (When it sent it's message)
    uint64_t timestamp_tx_2 = data.buffer.timestamp_tx;
    //Compute RX Timestamp (When it got our message)
    info->timestamp_rx_1 = time_sub(timestamp_tx_2, T_REPLY);

    //Compute Next Send Time
    uint64_t timestamp_tx_3 = time_add(timestamp_rx_2, T_REPLY);
    
    //Compose Response
    delay_time.delay_time = timestamp_tx_3;
    tx_buff->timestamp_tx = timestamp_tx_3;
    tx_buff->fp_ampl1 = data.timestamp.fp_ampl1;
    tx_buff->rxpacc_rx = data.finfo.rxpacc;
    tx_buff->fqual = data.fqual;
    //Write in new buffer
    spi_write_msg(buff_rx, tx_buff, TX_BUFFER_LEN);
    
    //Send Message
    send_message_delay(ctrl, &delay_time, 0);
    //debug_print("Message will be sent, I am done\n");
    info->timestamp_tx_1 = timestamp_tx_1;
    info->timestamp_tx_2 = timestamp_tx_2;
    info->timestamp_rx_2 = timestamp_rx_2;
    info->timestamp_tx_3 = timestamp_tx_3;
    //TODO: Propogation Logic
    /*
    uint64_t t_round1 = time_sub(timestamp_rx_2, timestamp_tx_1);
    uint64_t t_prop = (t_round1 - T_REPLY) / 2;
    printf("I compute a tProp of %llu clock cycles\n", t_prop);
    double prop_in_secs = ((double) t_prop) / ((double) CLOCK_FREQ);
    printf("So the distance is %f meters", prop_in_secs * LIGHT_SPEED);
    */
  }

void ranging_recv(struct system_control *ctrl,
    struct system_status *sta, struct tx_buffer *tx_buff, 
    struct range_info *info) {
   //Go into recieve moce - Wait for Message
   //debug_print("Ok, waiting for first message...\n");
   wait_for_msg_int(ctrl, sta);
   //debug_print("Got First Message, grabbing data...\n");
   
   //Grab Info
   struct rx_data data;
   get_rx_data(&data);
   //Get Timestamp
   info->timestamp_rx_1 = data.timestamp.rx_stamp; //Lower 40 Bits
   //Get TX Timestamp
   info->timestamp_tx_1 = data.buffer.timestamp_tx;
   //Compute Next Send Time
   info->timestamp_tx_2 = time_add(info->timestamp_rx_1, T_REPLY);
   //debug_print("Ok, got message at timestamp %llu, which was sent at"
   //" %llu, will send at %llu\n",
   // timestamp_rx_1, timestamp_tx_1, timestamp_rx_2);
   
   //Compose Response
   struct dx_time delay_time; 
   delay_time.reg = DX_TIME_REG | WRITE;
   delay_time.delay_time = info->timestamp_tx_2;
   tx_buff->timestamp_tx = info->timestamp_tx_2;
   tx_buff->fp_ampl1 = data.timestamp.fp_ampl1;
   tx_buff->rxpacc_rx = data.finfo.rxpacc;
   tx_buff->fqual = data.fqual;

   //Write in new buffer
   char buff_rx[TX_BUFFER_LEN];
   spi_write_msg(buff_rx, tx_buff, TX_BUFFER_LEN);
   
   //Send Message
   send_message_delay(ctrl, &delay_time, 1);
  
   //Store RX info
   info->rx_fqual_1 = data.fqual;
   info->rxpacc_rx_1 = data.finfo.rxpacc;
   info->fp_ampl1_rx_1 = data.timestamp.fp_ampl1;


   //Wait for Response
   wait_for_msg_int(ctrl, sta);
 
  //Grab Info
  struct rx_data data2;
  get_rx_data(&data2);
  //Get Timestamp
  info->timestamp_rx_3 = data2.timestamp.rx_stamp; //Lower 40 Bits
  //Get TX Timestamp
  info->timestamp_tx_3 = data2.buffer.timestamp_tx;
  info->timestamp_rx_2 = time_sub(info->timestamp_tx_3, T_REPLY);
  
  //Store RX info
  info->rx_fqual_2 = data2.buffer.fqual;
  info->fp_ampl1_rx_2 = data2.buffer.fp_ampl1;
  info->rxpacc_rx_2 = data2.buffer.rxpacc_rx;
  
  info->rx_fqual_3 = data2.fqual;
  info->rxpacc_rx_3 = data2.finfo.rxpacc;
  info->fp_ampl1_rx_3 = data2.timestamp.fp_ampl1;
  
  //TODO: Propogation Logic
  /*
  uint64_t timestamp_rx_2 = time_sub(timestamp_tx_3,T_REPLY);
  uint64_t t_round1 = time_sub(timestamp_rx_2, timestamp_tx_1);
  uint64_t t_round2 = time_sub(timestamp_rx_3, timestamp_tx_2);
  printf("The other device will compute tprop of %llu\n", (t_round1 - T_REPLY) / 2);
  double t_prop = ((double)t_round1 * (double)t_round2 - (double)T_REPLY*(double)T_REPLY) / ((double)t_round1 + (double)t_round2 + (double)T_REPLY + (double)T_REPLY);
  printf("I compute a tprop of %f clock cycles\n",t_prop);
  double prop_in_secs = ((double) t_prop) / ((double) CLOCK_FREQ);
  printf("So the distance is %f meters", prop_in_secs * LIGHT_SPEED);
  */
}