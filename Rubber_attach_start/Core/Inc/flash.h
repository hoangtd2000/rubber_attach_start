/*
 * flash.h
 *
 *  Created on: Jan 10, 2026
 *      Author: MCNEX
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"
uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);
void Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords);






#endif /* INC_FLASH_H_ */
