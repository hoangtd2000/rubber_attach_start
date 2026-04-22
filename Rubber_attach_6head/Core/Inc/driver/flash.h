/*
 * flash.h
 *
 *  Created on: Jan 10, 2026
 *      Author: MCNEX
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "stm32f4xx_hal.h"

uint32_t GetSector(uint32_t Address);

/* ===============================
   16-bit VERSION (ACTIVE)
   =============================== */
void Flash_Read_Data (uint32_t StartAddress, uint16_t *RxBuf, uint16_t NumberOfWords);
uint32_t Flash_Write_Data (uint32_t StartAddress, uint16_t *Data, uint16_t NumberOfWords);

/* ===============================
   64-bit VERSION (COMMENTED)
   =============================== */
// void Flash_Read_Data_64bit (uint32_t StartAddress, uint64_t *RxBuf, uint16_t NumberOfDoubleWords);
// uint32_t Flash_Write_Data_64bit (uint32_t StartAddress, uint64_t *Data, uint16_t NumberOfDoubleWords);


#endif /* INC_FLASH_H_ */
