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

uint8_t Timer_Check(uint8_t id, uint32_t interval)
{
    static uint32_t previousMillis[10] = {0};

    if (Tick - previousMillis[id] >= interval)
    {
        previousMillis[id] = Tick;
        return 1;
    }
    return 0;
}
