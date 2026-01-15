/*
 * IO_Controller.c
 *
 *  Created on: Jan 14, 2026
 *      Author: CongChuc
 */


#include "IO_Controller.h"
uint8_t NumberBips = 0;
uint8_t modeBip = 1;
uint16_t TIME_SET_BIP_ON = 1000;
uint16_t TIME_SET_BIP_OFF = 500;
uint16_t TimeDelayBip = 0;
void SetBips(uint8_t numBips){
	NumberBips = numBips;
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

void SetBlinks(uint8_t numBlinks){
	for(uint8_t i = 0; i < numBlinks; i++){
		HAL_GPIO_TogglePin(O11_GPIO_Port, O11_Pin);
		delay_us(500);
	}
}

uint8_t Pick_Item(uint8_t No){
	if(No == 1){
		for(uint8_t i = 0; i < 3; i++){
			Cylinder1_Go_Down;
			HAL_Delay(500);
			Vacum1_Pick;
			HAL_Delay(500);
			Cylinder1_Go_Up;
//			if(Is_Vacum1_Pick == 0){
//				return 1;
//			}
		}
		return 0;
	}
	else if(No == 2){
		for(uint8_t i = 0; i < 3; i++){
			Cylinder2_Go_Down;
			HAL_Delay(500);
			Vacum2_Pick;
			HAL_Delay(500);
			Cylinder2_Go_Up;
//			if(Is_Vacum2_Pick == 0){
//				return 1;
//			}
		}
		return 0;
	}
	return 0;
}
