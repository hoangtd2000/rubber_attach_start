/*
 * Tick.h
 *
 *  Created on: Jan 12, 2026
 *      Author: MCNEX
 */

#ifndef INC_TICK_H_
#define INC_TICK_H_

#include "stdint.h"
void delay_us(uint32_t us);
uint32_t millis(void);
uint8_t Timer_Check(uint8_t id, uint32_t interval);
#endif /* INC_TICK_H_ */
