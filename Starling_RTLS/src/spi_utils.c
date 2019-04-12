#include <stdio.h>
#include <unistd.h>

#include "spi_comms.h"
#include "deca_constants.h"
#include "deca_structs.h"
#include "deca_init.h"
#include "asf.h"
#include "spi_starling.h"
#include <string.h>
#define STDOUT_FD 1

//Interrupt Flag
volatile int interrupt_flag = 0;

void gpio_interrupt(void);

void gpio_interrupt(void){
	interrupt_flag = 1;
}

void send_message_delay(struct system_control *ctrl,
						struct dx_time *delay_time,
						int reenb_transmitter) {
    //Write in delay time
    char rx_dx_time[DX_TIME_LEN] = {0x00};
    delay_time->reg = DX_TIME_REG | WRITE;
    spi_write_msg(rx_dx_time, delay_time, DX_TIME_LEN);
    //Setup System Control to send 
    sys_ctrl_init(ctrl); 
    ctrl->txstrt = 0x01; //Set sending flag
    ctrl->txdlys = 0x01; //Set delayed send
    ctrl->waitresp = reenb_transmitter; //Automatically wait for next msg
    unsigned char rx_sys_ctrl[SYS_CTRL_LEN];
    spi_write_msg(rx_sys_ctrl, ctrl, SYS_CTRL_LEN); //IT WILL BE SENT
    return;
  }

void set_pll(void) {
    struct ec_ctrl pll_ctrl;
    pll_ctrl.reg = EC_CTRL_REG | WRITE;
    pll_ctrl.ostsm = 0x00;
    pll_ctrl.osrsm = 0x00;
    pll_ctrl.pllldt = 0x01; //Set
    pll_ctrl.wait = 0x00;
    pll_ctrl.ostrm = 0x00;
    pll_ctrl.res = 0x00;
    char ec_ctrl_rx[EC_CTRL_LEN];
    spi_write_msg(ec_ctrl_rx, &pll_ctrl, EC_CTRL_LEN);
}

void wait_for_msg_int(struct system_control *ctrl,
    struct system_status *sta) {
    //Clear Status Reg  
    //clear_status(bus, sta);
	    
    //Turn on receiver
    sys_ctrl_init(ctrl); 
    unsigned char rx_sys_ctrl[SYS_CTRL_LEN] = {0x00};
    ctrl->rxenab = 0x01;
    spi_write_msg(rx_sys_ctrl, ctrl, SYS_CTRL_LEN); //RECEIVER ON
    
    //Wait for GPIO Interrpt
    //TODO: Interrupt, for now we will poll
    unsigned char tx_status[SYS_STATUS_LEN] = {0x00};
	unsigned char rx_status[SYS_STATUS_LEN] = {0x00};
    tx_status[0] = SYS_STATUS_REG;
    while (1) {
      spi_write_msg(sta, tx_status, SYS_STATUS_LEN); 
      memcpy(rx_status, sta, SYS_STATUS_LEN);
  //    debug_print("0x%02X 0x%02X 0x%02X 0x%02X\n", rx_status[1]
  //    ,rx_status[2], rx_status[3], rx_status[4]);
      if (sta->rxdfr) {  //Check Data Ready Status 
        //Clear Status Pin
        clear_status(sta);
        break;
      }
	  delay_us(10);
    }

    /*
    wiringPiISR(interrupt_pin, INT_EDGE_RISING, gpio_interrupt);
    //Wait for Interrupt
    while(interrupt_flag == 0) {};
    //Clear Flag
    interrupt_flag = 0;
    */
}

void wait_for_msg(struct system_control *ctrl) {

    //Turn on receiver
    sys_ctrl_init(ctrl); 
    unsigned char rx_sys_ctrl[SYS_CTRL_LEN] = {0x00};
    ctrl->rxenab = 0x01;
    spi_write_msg(rx_sys_ctrl, ctrl, SYS_CTRL_LEN); //RECEIVER ON
	delay_ms(1000);
    
    //Wait for msg
    //TODO: Interrupts, for now, we will poll
    //Clear Status Register 
    //Poll Status Event Register

    struct system_status sta;
    unsigned char tx_status[SYS_STATUS_LEN] = {0x00};
    unsigned char rx_status[SYS_STATUS_LEN] = {0x00};

    unsigned char rx_buffer_tx[RX_BUFFER_LEN] = {0x00};
    rx_buffer_tx[0] = RX_BUFFER_REG;
    
    while (1) {
      tx_status[0] = SYS_STATUS_REG;
      spi_write_msg(rx_status, tx_status, SYS_STATUS_LEN);
      //Check if there is data 
      //debug_print("0x%02X 0x%02X 0x%02X 0x%02X\n", rx_status[1]
      //  ,rx_status[2], rx_status[3], rx_status[4]);
      if (rx_status[2] & 0x20) {  //Check Data Ready Status
        //Clear Status Pin
        clear_status(&sta);
        //Get Message
        struct rx_buffer recv_buff;
        spi_write_msg(&recv_buff, rx_buffer_tx, RX_BUFFER_LEN);
        //Write Message to stdout
        //write(STDOUT_FD, recv_buff.payload, PAYLOAD_LEN);
        break;
      }
      delay_ms(1000);
    }
}

  //Checks that we can read the device ID
  //Returns 1 if we can, 0 if we cannot
int comms_check(void) {
    unsigned char rx_buf[DEV_ID_LEN] = {0x00};
    unsigned char tx_buf[DEV_ID_LEN] = {0x00};
    spi_write_msg(rx_buf, tx_buf, DEV_ID_LEN);
//     debug_print("0x%02X 0x%02X 0x%02X 0x%02X\n",
//            rx_buf[1], rx_buf[2], rx_buf[3], rx_buf[4]);
    return (rx_buf[1] == 0x30) && (rx_buf[2] == 0x01)
            && (rx_buf[3] == 0xCA) && (rx_buf[4] == 0xDE);
}

uint64_t time_add(uint64_t curr_time, uint64_t delay_time) {
  //debug_print("The Current time is %llu\n", curr_time);
  //debug_print("We want to delay by %llu\n", delay_time);
  uint64_t sum_time = curr_time + delay_time;
  //Wrap around if we overflow
  if (sum_time > ((uint64_t) 0xFFFFFFFFFF)) {
    uint64_t wrap_time = delay_time - ((uint64_t) 0xFFFFFFFFFF - sum_time);
    return wrap_time;
  } else {
   //debug_print("No overflow, using time as is\n");
   return sum_time;
  }
}

//Computes t_larger - t_smaller
uint64_t time_sub(uint64_t t_larger, uint64_t t_smaller) {
  if (t_smaller > t_larger) //If we wrapped around
    return ((uint64_t) 0xFFFFFFFFFF - t_smaller) + t_larger;
  else
    return t_larger - t_smaller;
}

int get_rx_data(struct rx_data *data) {
  //Initialize structs
  data->finfo.reg = RX_FINFO_REG;
  data->timestamp.reg = RX_TIME_REG;
  data->buffer.reg = RX_BUFFER_REG;
  data->fqual.reg = RX_FQUAL_REG;
  //Initalize TX Messages
  char tx_finfo[RX_FINFO_LEN] = {0x00};
  tx_finfo[0] = RX_FINFO_REG;
  char tx_timestamp[RX_TIME_LEN] = {0x00};
  tx_timestamp[0] = RX_TIME_REG;
  char tx_buffer[TX_BUFFER_LEN] = {0x00};
  tx_buffer[0] = RX_BUFFER_REG;
  char tx_fqual[RX_FQUAL_LEN] = {0x00};
  tx_fqual[0] = RX_FQUAL_REG;
  //Write Data Into Structs
  spi_write_msg(&(data->finfo), tx_finfo, RX_FINFO_LEN);
  spi_write_msg(&(data->timestamp), tx_timestamp, RX_TIME_LEN);
  spi_write_msg(&(data->buffer), tx_buffer, RX_BUFFER_LEN);
  spi_write_msg(&(data->fqual), tx_fqual, RX_FQUAL_LEN);
  return 0;
}

void clear_status(struct system_status *sta) {
//Clears all RX/TX System Status Flags
  sta->reg = SYS_STATUS_REG | WRITE;
  sta->irqs = 0;
  sta->cplock = 0;
  sta->esyncr = 0;
  sta->aat = 0;
  sta->txfrb = 1;
  sta->txprs = 1;
  sta->txphs = 1;
  sta->txfrs = 1;
  sta->rxprd = 1;
  sta->rxsfdd = 1;
  sta->ldedone = 0;
  sta->rxphd = 1;
  sta->rxphe = 1;
  sta->rxdfr = 1;
  sta->rxfcg = 1;
  sta->rxfce = 1;
  sta->rxrfsl = 1;
  sta->rxrfto = 1;
  sta->ldeerr = 0;
  sta->res = 0;
  sta->rxovrr = 0;
  sta->rxpto = 1;
  sta->gpioirq = 1;
  sta->slpinit = 0;
  sta->rfpll = 1;
  sta->clkpll = 1;
  sta->rxsfdto = 1;
  sta->hpdwarn = 0;
  sta->txberr = 1;
  sta->affrej = 1;
  sta->hsrbp = 0;
  sta->icrbp = 0;
  sta->rxrscs = 1;
  sta->rxprej = 1;
  sta->txpute = 0;
  sta->res_2 = 0;
  unsigned char rx_buf[SYS_STATUS_LEN];
  spi_write_msg(rx_buf, sta, SYS_STATUS_LEN);
}

void reset_trx(struct system_control *ctrl) {
  sys_ctrl_init(ctrl);
  char ctrl_rx[SYS_CTRL_LEN] = {0x00};
  ctrl->trxoff = 0x01;
  spi_write_msg(&ctrl_rx, ctrl, SYS_CTRL_LEN);
  ctrl->trxoff = 0x00;
}
