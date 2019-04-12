/*
 * delay_starling.h
 *
 * Created: 3/21/2019 5:54:35 PM
 *  Author: Aaron
 */ 


#ifndef DELAY_STARLING_H_
#define DELAY_STARLING_H_

// note we can only delay in increments of ~9 ns.
// For delays of less than several 10s of nanoseconds, 
// inserting a couple of asm("nop")'s is probably better
// as there is loop overhead here that makes the delay
// not particularly accurate
inline void delay_ns(uint16_t ns){
	uint16_t cycles = (uint16_t) ns/8.33;
	for (int ix = 0; ix < cycles/3; ix++){
		asm("nop");
	}
}



#endif /* DELAY_STARLING_H_ */