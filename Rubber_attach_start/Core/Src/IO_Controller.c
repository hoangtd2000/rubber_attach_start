/*
 * IO_Controller.c
 *
 *  Created on: Jan 14, 2026
 *      Author: CongChuc
 */


#include "IO_Controller.h"
uint8_t NumberBips = 0;
uint8_t modeBip = 0;
uint16_t TIME_SET_BIP_ON = 500;
uint16_t TIME_SET_BIP_OFF = 500;
uint16_t TimeDelayBip = 0;

Cylinder_Vacum_Init_t Handle_Pick;
Cylinder_Vacum_Init_t Handle_Release;

void SetBips(uint8_t numBips){
	NumberBips = numBips - 1;
}

void BipControl(void){
	switch(modeBip){
		case 0:
			ON_BUZZ;
			TimeDelayBip = TIME_SET_BIP_ON;
			modeBip = 1;
			break;
		case 1:
			if(TimeDelayBip == 0){
				OFF_BUZZ;
				TimeDelayBip = TIME_SET_BIP_OFF;
				modeBip = 2;
			}
			break;
		case 2:
			if(TimeDelayBip == 0){
				modeBip = 0;
				if(NumberBips > 0) NumberBips--;
				else modeBip = 3;
			}
			break;
		case 3:
			if(NumberBips != 0)
				modeBip = 0;
			break;
		default:
			break;
	}
	if(TimeDelayBip > 0) TimeDelayBip--;
}

uint8_t PickRubber(uint8_t vacum_id)
{
    for(int retry = 0; retry < 2; retry++)
    {
        if(vacum_id == 1){
        	Cylinder1_Go_Down;
            delay_us(100); // Thời gian xi lanh hạ
            Vacum1_Release_Off;
            delay_us(50);
            Vacum1_Pick_On;
            delay_us(100); // Thời gian hút
            Cylinder1_Go_Up;
            delay_us(100); // Thời gian xi lanh nâng
            if(Is_Vacum1_Pick) return 1;
        }
        else{
            Cylinder2_Go_Down;
            delay_us(100);  // Thời gian xi lanh hạ
            Vacum2_Release_Off;
            delay_us(50);
            Vacum2_Pick_On;
            delay_us(100);  // Thời gian hút
            Cylinder2_Go_Up;
            delay_us(100); // Thời gian xi lanh nâng
            if(Is_Vacum2_Pick) return 1;
        }
    }
    return 0;
}

uint8_t ReleaseRubber(uint8_t vacum_id)
{
    for(int retry = 0; retry < 2; retry++)
    {
        if(vacum_id == 1){
            Cylinder1_Go_Down;
            delay_us(100); // Thời gian xi lanh hạ
            Vacum1_Pick_Off;
            delay_us(50);
            Vacum1_Release_On;
            delay_us(100); // Thời gian nhả
            Cylinder1_Go_Up;
            delay_us(100); // Thời gian xi lanh nâng
            if(Is_Vacum1_Pick == 0) return 1;
        }
        else{
            Cylinder2_Go_Down;
            delay_us(100); // Thời gian xi lanh hạ
            Vacum2_Pick_Off;
            delay_us(50);
            Vacum2_Release_On;
            delay_us(100); // Thời gian nhả
            Cylinder2_Go_Up;
            delay_us(100); // Thời gian xi lanh nâng
            if(Is_Vacum2_Pick == 0) return 1;
        }
    }
    return 0;
}
