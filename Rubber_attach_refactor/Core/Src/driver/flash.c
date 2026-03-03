/*
 * flash.c
 *
 *  Created on: Jan 10, 2026
 *      Author: MCNEX
 */


#include "flash.h"


uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if(Address < 0x08004000) sector = FLASH_SECTOR_0;
  else if(Address < 0x08008000) sector = FLASH_SECTOR_1;
  else if(Address < 0x0800C000) sector = FLASH_SECTOR_2;
  else if(Address < 0x08010000) sector = FLASH_SECTOR_3;
  else if(Address < 0x08020000) sector = FLASH_SECTOR_4;
  else if(Address < 0x08040000) sector = FLASH_SECTOR_5;
  else if(Address < 0x08060000) sector = FLASH_SECTOR_6;
  else if(Address < 0x08080000) sector = FLASH_SECTOR_7;
  else sector = FLASH_SECTOR_7;
  return sector;
}
//void Flash_Read_Data (uint32_t StartSectorAddress, uint64_t *RxBuf, uint16_t numberofwords)
//{
//	while (1)
//	{
//		*RxBuf = *(__IO uint32_t *)StartSectorAddress;
//		StartSectorAddress += 4;
//		RxBuf++;
//		if (!(numberofwords--)) break;
//	}
//}

/* =========================
   2. READ 64-bit CORRECT
   ========================= */
void Flash_Read_Data(uint32_t StartAddress,
                     uint64_t *RxBuf,
                     uint16_t NumberOfDoubleWords)
{
    for(uint16_t i = 0; i < NumberOfDoubleWords; i++)
    {
        uint32_t low  = *(__IO uint32_t*)StartAddress;
        StartAddress += 4;

        uint32_t high = *(__IO uint32_t*)StartAddress;
        StartAddress += 4;

        RxBuf[i] = ((uint64_t)high << 32) | low;
    }
}
//uint32_t Flash_Write_Data(uint32_t StartAddress,
//                          uint64_t *Data,
//                          uint16_t NumberOfWords)
//{
//    FLASH_EraseInitTypeDef EraseInitStruct;
//    uint32_t SectorError = 0;
//    uint32_t addr = StartAddress;
//
//    /* 1. Disable interrupt để tránh UART/Modbus ISR nhảy vào Flash */
//    __disable_irq();
//
//    /* 2. Unlock Flash */
//    HAL_FLASH_Unlock();
//
//    /* 3. Xác định sector cần erase (CHỈ 1 SECTOR) */
//    uint32_t StartSector = GetSector(StartAddress);
//
//    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
//    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
//    EraseInitStruct.Sector       = StartSector;
//    EraseInitStruct.NbSectors    = 1;   // <<< RẤT QUAN TRỌNG
//
//    /* 4. Erase sector */
//    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
//    {
//        HAL_FLASH_Lock();
//        __enable_irq();
//        return HAL_FLASH_GetError();
//    }
//
//    /* 5. Program word-by-word */ // FLASH_TYPEPROGRAM_HALFWORD
//    for (uint16_t i = 0; i < NumberOfWords; i++)
//    {
//        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, Data[i]) != HAL_OK)
//        {
//            HAL_FLASH_Lock();
//            __enable_irq();
//            return HAL_FLASH_GetError();
//        }
//        addr += 4;
//    }
//
//    /* 6. Lock Flash */
//    HAL_FLASH_Lock();
//
//    /* 7. Flush cache – BẮT BUỘC theo ST */
//    __HAL_FLASH_DATA_CACHE_DISABLE();
//    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
//
//    __HAL_FLASH_DATA_CACHE_RESET();
//    __HAL_FLASH_INSTRUCTION_CACHE_RESET();
//
//    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
//    __HAL_FLASH_DATA_CACHE_ENABLE();
//
//    /* 8. Enable lại interrupt */
//    __enable_irq();
//
//    return 0;
//}
/* =========================
   3. WRITE 64-bit SAFE
   ========================= */
uint32_t Flash_Write_Data(uint32_t StartAddress,
                          uint64_t *Data,
                          uint16_t NumberOfDoubleWords)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;
    uint32_t addr = StartAddress;

    /* 0. Bắt buộc align 8 byte */
    if (addr % 8 != 0)
        return HAL_ERROR;

    __disable_irq();
    HAL_FLASH_Unlock();

    /* 1. Erase đúng sector */
    uint32_t StartSector = GetSector(StartAddress);

    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector       = StartSector;
    EraseInitStruct.NbSectors    = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        __enable_irq();
        return HAL_FLASH_GetError();
    }

    /* 2. Ghi từng double word */
    for (uint16_t i = 0; i < NumberOfDoubleWords; i++)
    {
        uint32_t low  = (uint32_t)(Data[i] & 0xFFFFFFFF);
        uint32_t high = (uint32_t)((Data[i] >> 32) & 0xFFFFFFFF);

        /* Ghi 32-bit thấp */
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, low) != HAL_OK)
            goto error;

        addr += 4;

        /* Ghi 32-bit cao */
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, high) != HAL_OK)
            goto error;

        addr += 4;

        /* Verify */
        if (*(uint64_t*)(addr - 8) != Data[i])
            goto error;
    }

    HAL_FLASH_Lock();

    /* Flush cache */
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();

    __enable_irq();
    return 0;

error:
    HAL_FLASH_Lock();
    __enable_irq();
    return HAL_FLASH_GetError();
}
