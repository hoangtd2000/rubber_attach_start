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

uint8_t NumberBlinks = 0;
uint8_t modeBlink = 0;
uint16_t TIME_SET_BLINK_ON = 500;
uint16_t TIME_SET_BLINK_OFF = 500;
uint16_t TimeDelayBlink = 0;

void SetBips(uint8_t numBips){
	NumberBips = numBips - 1;
}
void SetBlinks(uint8_t numBlinks){
	NumberBlinks = numBlinks - 1;
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

void BlinkControl(){
	switch(modeBlink){
		case 0:
			ON_LED_RED;
			TimeDelayBlink = TIME_SET_BLINK_ON;
			modeBlink = 1;
			break;
		case 1:
			if(TimeDelayBlink == 0){
				OFF_LED_RED;
				TimeDelayBlink = TIME_SET_BLINK_OFF;
				modeBlink = 2;
			}
			break;
		case 2:
			if(TimeDelayBlink == 0){
				modeBlink = 0;
				if(NumberBlinks > 0) NumberBlinks--;
				else modeBlink = 3;
			}
			break;
		case 3:
			if(NumberBlinks != 0)
				modeBlink = 0;
			break;
		default:
			break;
	}
	if(TimeDelayBlink > 0) TimeDelayBlink--;
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
            delay_us(200); // Thời gian hút
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
            delay_us(200);  // Thời gian hút
            Cylinder2_Go_Up;
            delay_us(100); // Thời gian xi lanh nâng
            if(Is_Vacum2_Pick) return 1;
        }
    }
    //Set_Warring(0);
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
            delay_us(200); // Thời gian nhả
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
            delay_us(200); // Thời gian nhả
            Cylinder2_Go_Up;
            delay_us(100); // Thời gian xi lanh nâng
            if(Is_Vacum2_Pick == 0) return 1;
        }
    }
    return 0;
}
