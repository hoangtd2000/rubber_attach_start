/*
 * modbusRTU.h
 *
 *  Created on: Oct 11, 2025
 *      Author: Admin
 */

#ifndef INC_MODBUSRTU_H_
#define INC_MODBUSRTU_H_

#include "stm32f4xx_hal.h"
#include <string.h>

uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);
#define SLAVE_ID 1

#define ILLEGAL_FUNCTION       	0x01
#define ILLEGAL_DATA_ADDRESS   	0x02
#define ILLEGAL_DATA_VALUE     	0x03
#define Min_NumCoils 			(1U)
#define Max_NumCoils 			(49U)
#define Min_NumRegs 			(1U)
#define Max_NumRegs 			(299U)

uint8_t readHoldingRegs (void);
uint8_t readInputRegs (void);
uint8_t readCoils (void);
uint8_t readInputs (void);

uint8_t writeSingleReg (void);
uint8_t writeHoldingRegs (void);
uint8_t writeSingleCoil (void);
uint8_t writeMultiCoils (void);

void modbusException (uint8_t exceptioncode);

#endif /* INC_MODBUSRTU_H_ */
