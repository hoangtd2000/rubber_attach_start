/*
 * modbus_rtu.h
 *
 *  Created on: Jan 24, 2026
 *      Author: Admin
 */

#ifndef INC_DRIVER_MODBUS_RTU_H_
#define INC_DRIVER_MODBUS_RTU_H_

#include "stm32f4xx_hal.h"
#include <string.h>

#define SLAVE_ID 				(1U)
#define ILLEGAL_FUNCTION       	(0x01)
#define ILLEGAL_DATA_ADDRESS   	(0x02)
#define ILLEGAL_DATA_VALUE     	(0x03)
#define Min_NumCoils 			(1U)
#define Max_NumCoils 			(49U)
#define Min_NumRegs 			(1U)
#define Max_NumRegs 			(299U)

extern UART_HandleTypeDef huart2;
extern uint8_t Coils_Database[25];
extern uint16_t Holding_Registers_Database[300];
extern uint8_t Inputs_Database[50];
extern uint16_t Input_Registers_Database[50];

uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);
uint8_t readHoldingRegs (void);
uint8_t readInputRegs (void);
uint8_t readCoils (void);
uint8_t readInputs (void);

uint8_t writeSingleReg (void);
uint8_t writeHoldingRegs (void);
uint8_t writeSingleCoil (void);
uint8_t writeMultiCoils (void);

void modbusException (uint8_t exceptioncode);

#endif /* INC_DRIVER_MODBUS_RTU_H_ */
