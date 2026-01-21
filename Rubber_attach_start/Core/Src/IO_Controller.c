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
Cylinder_Vacum_Init_t Handle_Realse;

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

void SetPickRubber(uint8_t vacum_id) // vacum_id 1 or 2
{
    if (vacum_id >= 2) return;

    Handle_Pick.mode       = MODE_PICK;
    Handle_Pick.vacum_id   = vacum_id;
    Handle_Pick.retry      = 0;
    Handle_Pick.time_delay = 0;
    Handle_Pick.result     = 0;
    Handle_Pick.state      = CYLINDER_GO_DOWN;
}

void SetReleaseRubber(uint8_t vacum_id){
    if (vacum_id >= 2) return;

    Handle_Realse.mode       = MODE_RELEASE;
    Handle_Realse.vacum_id   = vacum_id;
    Handle_Realse.retry      = 0;
    Handle_Realse.time_delay = 0;
    Handle_Realse.result     = 0;
    Handle_Realse.state      = CYLINDER_GO_DOWN;
}

void PickRubber(void)
{
    switch (Handle_Pick.state)
    {
    case PICK_IDLE:
        // không làm gì, chờ SetPickRubber()
        break;

    case CYLINDER_GO_DOWN:
    	Handle_Pick.retry = 0;
        if (Handle_Pick.vacum_id == 1)  Cylinder1_Go_Down;
        else                					Cylinder2_Go_Down;
        Handle_Pick.time_delay = 100;
        Handle_Pick.state = VACUM_REALSE_OFF;
        break;

    case VACUM_REALSE_OFF:
        if (Handle_Pick.time_delay == 0)
        {
            if (Handle_Pick.vacum_id == 1)  Vacum1_Release_Off;
            else                					Vacum2_Release_Off;
            Handle_Pick.time_delay = 50;
            Handle_Pick.state = VACUM_PICK_ON;
        }
        break;

    case VACUM_PICK_ON:
        if (Handle_Pick.time_delay == 0)
        {
            if (Handle_Pick.vacum_id == 1) Vacum1_Pick_On;
            else                                   Vacum2_Pick_On;

            Handle_Pick.time_delay = 100;
            Handle_Pick.state = CYLINDER_GO_UP;
        }
        break;

    case CYLINDER_GO_UP:
        if (Handle_Pick.time_delay == 0)
        {
            if (Handle_Pick.vacum_id == 1) Cylinder1_Go_Up;
            else                                   Cylinder2_Go_Up;
            Handle_Pick.time_delay = 100;
            Handle_Pick.state = PICK_RETRY;
        }
        break;

    case PICK_RETRY:
        if (Handle_Pick.time_delay == 0)
        {
        	uint8_t check;
        	if (Handle_Pick.vacum_id == 1)
        	    check = Is_Vacum1_Pick;
        	else
        	    check = Is_Vacum2_Pick;

            if (check)
            {
            	Handle_Pick.state = PICK_DONE_OK;
            }
            else
            {
                if (++Handle_Pick.retry < 2)
                	Handle_Pick.state = CYLINDER_GO_DOWN;
                else
                	Handle_Pick.state = PICK_DONE_NG;
            }
        }
        break;

    case PICK_DONE_OK:
    	Handle_Pick.result = 1;          // báo OK
    	Handle_Pick.state = PICK_IDLE;
        break;

    case PICK_DONE_NG:
    	Handle_Pick.result = 0;          // báo NG
    	Handle_Pick.state = PICK_IDLE;
        break;
    }

    if (Handle_Pick.time_delay > 0) Handle_Pick.time_delay--;
}

void ReleaseRubber(void)
{
    switch (Handle_Realse.state)
    {
    case PICK_IDLE:
        // không làm gì, chờ SetPickRubber()
        break;

    case CYLINDER_GO_DOWN:
    	Handle_Realse.retry = 0;
        if (Handle_Realse.time_delay == 1)  Cylinder1_Go_Down;
        else                				Cylinder2_Go_Down;
        Handle_Realse.time_delay = 100;
        Handle_Realse.state = VACUM_PICK_OFF;
        break;

    case VACUM_PICK_OFF:
        if (Handle_Realse.time_delay == 0)
        {
            if (Handle_Realse.vacum_id == 1)  Vacum1_Pick_Off;
            else                			  Vacum2_Pick_Off;
            Handle_Realse.time_delay = 50;
            Handle_Realse.state = VACUM_REALSE_ON;
        }
        break;

    case VACUM_REALSE_ON:
        if (Handle_Realse.time_delay == 0)
        {
            if (Handle_Realse.vacum_id == 1)
                Vacum1_Release_On;
            else
                Vacum2_Release_On;

            Handle_Realse.time_delay = 100;
            Handle_Realse.state = CYLINDER_GO_UP;
        }
        break;

    case CYLINDER_GO_UP:
        if (Handle_Realse.time_delay == 0)
        {
            if (Handle_Realse.vacum_id == 1)
                Cylinder1_Go_Up;
            else
                Cylinder2_Go_Up;

            Handle_Realse.time_delay = 100;
            Handle_Realse.state = PICK_RETRY;
        }
        break;

    case PICK_RETRY:
        if (Handle_Realse.time_delay == 0)
        {
        	uint8_t ok;

        	if (Handle_Realse.vacum_id == 1)
        	    ok = Is_Vacum1_Pick;
        	else
        	    ok = Is_Vacum2_Pick;

            if (ok)
            {
            	Handle_Realse.state = PICK_DONE_OK;
            }
            else
            {
                if (++Handle_Realse.retry < 2)
                	Handle_Realse.state = CYLINDER_GO_DOWN;
                else
                	Handle_Realse.state = PICK_DONE_NG;
            }
        }
        break;

    case PICK_DONE_OK:
    	Handle_Realse.result = 1;          // báo OK
    	Handle_Realse.state = PICK_IDLE;
        break;

    case PICK_DONE_NG:
    	Handle_Realse.result = 0;          // báo NG
    	Handle_Realse.state = PICK_IDLE;
        break;
    }

    if (Handle_Realse.time_delay > 0) Handle_Realse.time_delay--;
}

uint8_t PickRubber1(uint8_t vacum_id)
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

uint8_t ReleaseRubber1(uint8_t vacum_id)
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
