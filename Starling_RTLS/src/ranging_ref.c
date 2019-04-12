// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <time.h>
// 
// #include "spi_comms.h"
// 
// volatile sig_atomic_t interrupt_test = 0;
// 
// void test_interrupt(void) {
//   interrupt_test = 1;
// }
// 
// int main() {
// 
//   int spi_setup_error = spi_ranging_setup();
//    // NOTE: I think we need to get the initialized transmit/received buffers back from this + sys_ctrl
//      
//   while(1) {
//    //Determine Mode
//    printf("Mode: 1 for initial sender | 0 for receiver\n");
//    char ret = fgets((char *) &tx_buff.payload, sizeof(tx_buff.payload), stdin);
//    if (ret == NULL) {
//      printf("Error reading tx_buff.payload or no value entered\n");
//      exit(EXIT_FAILURE);
//    }
//    char i_am_sender = 1;
//    tx_buf.payload[0] = i_am_sender;
//   
//    struct range_info info;
//    memset(&info, 0x00, sizeof(struct range_info));
// 
//    //Check Mode
//    if (tx_buff.payload[0] == '0') {
// 
//      while (1) {
// 		ranging_recv(&sys_ctrl, &sta, &tx_buff, &info);
// 		//Propogation Logic
// 		/*
// 		uint64_t t_round1 = time_sub(info.timestamp_rx_2, info.timestamp_tx_1);
// 		uint64_t t_round2 = time_sub(info.timestamp_rx_3, info.timestamp_tx_2);
// 		double t_prop = ((double)t_round1 * (double)t_round2 - (double)T_REPLY*(double)T_REPLY) / ((double)t_round1 + (double)t_round2 + (double)T_REPLY + (double)T_REPLY);
// 		printf("I compute a tprop of %f clock cycles\n", t_prop);
// 		double prop_in_secs = ((double) t_prop) / ((double) CLOCK_FREQ);
// 		printf("So the distance is %f meters\n", prop_in_secs * LIGHT_SPEED);
// 		*/
// 		delay_ms(100);
//      }
//   } else if (tx_buff.payload[0] == '1') { //Transmit Message
// 
//       while (1) {
//         ranging_send(&sys_ctrl, &sta, &tx_buff, &info); 
//         //uint64_t t_round1 = time_sub(info.timestamp_rx_2, info.timestamp_tx_1);
//         //uint64_t t_prop = (t_round1 - T_REPLY) / 2;
//         //printf("I compute a tProp of %llu clock cycles\n", t_prop);
//         //double prop_in_secs = ((double) t_prop) / ((double) CLOCK_FREQ);
//         //printf("So the distance is %f meters\n", prop_in_secs * LIGHT_SPEED);
// 		delay_ms(1000);
//      }
//    }
//  }
//  return 0;
// }
