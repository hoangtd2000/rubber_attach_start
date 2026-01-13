/*
 * common.c
 *
 *  Created on: Jan 13, 2026
 *      Author: MCNEX
 */


#include"common.h"


uint32_t ReadBit(uint8_t* number, uint16_t startbit, uint8_t len)
{
	uint16_t byteIndex = startbit / 8;
	uint8_t bitPos = startbit % 8;

    uint32_t mask = ((1U << len) - 1U) << bitPos;
    return (number[byteIndex] & mask) >> bitPos;
}

// Ghi len bit liên tiếp, bắt đầu từ startbit, bằng level (0 hoặc 1)
void SetBit(uint8_t *number, uint16_t startbit, uint8_t len)
{
    uint32_t endbit = startbit + len;
    for (uint32_t bit = startbit; bit < endbit; ++bit)
    {
        uint32_t byteIndex = bit / 8;
        uint8_t bitPos = bit % 8;
        number[byteIndex] |= (1U << bitPos);
    }
}
void ClearBit(uint8_t *number, uint16_t startbit, uint8_t len)
{
    uint16_t endbit = startbit + len;
    for (uint16_t bit = startbit; bit < endbit; ++bit)
    {
        uint16_t byteIndex = bit / 8;
        uint8_t bitPos = bit % 8;
        number[byteIndex] &= ~(1U << bitPos);
    }
}

