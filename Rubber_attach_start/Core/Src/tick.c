/*
 * Tick.c
 *
 *  Created on: Jan 12, 2026
 *      Author: MCNEX
 */

#include <tick.h>
extern uint32_t Tick;
void delay_us(uint32_t us)
{
	  uint32_t tmp = Tick + us;
	    while(Tick < tmp){
	  	//  __NOP();
	    }

}
