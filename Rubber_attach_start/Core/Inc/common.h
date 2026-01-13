/*
 * Common.h
 *
 *  Created on: Jan 13, 2026
 *      Author: MCNEX
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include <stdint.h>
uint32_t ReadBit(uint8_t* number, uint16_t startbit, uint8_t len);
void SetBit(uint8_t* number, uint16_t startbit, uint8_t len);
void ClearBit(uint8_t* number, uint16_t startbit, uint8_t len);

#endif /* INC_COMMON_H_ */
