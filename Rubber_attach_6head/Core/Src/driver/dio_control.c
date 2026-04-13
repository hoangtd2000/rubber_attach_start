/*
 * IO_Controller.c
 *
 *  Created on: Jan 14, 2026
 *      Author: CongChuc
 */


#include <dio_control.h>
volatile uint8_t NumberBips = 0;
volatile uint8_t modeBip = 1;
volatile uint16_t TIME_SET_BIP_ON = 500;
volatile uint16_t TIME_SET_BIP_OFF = 500;
volatile uint16_t TimeDelayBip = 0;

Cylinder_Vacum_Init_t Handle_Pick[6];
Cylinder_Vacum_Init_t Handle_Release[6];

typedef struct
{
    GPIO_TypeDef *down_port;
    uint16_t down_pin;

    GPIO_TypeDef *pick_port;
    uint16_t pick_pin;

} HandlerIO;

HandlerIO handler_io[6] =
{
	{O1_GPIO_Port, O1_Pin, O7_GPIO_Port, O7_Pin},
	{O2_GPIO_Port, O2_Pin, O8_GPIO_Port, O8_Pin},
	{O3_GPIO_Port, O3_Pin, O9_GPIO_Port, O9_Pin},
	{O4_GPIO_Port, O4_Pin, O10_GPIO_Port, O10_Pin},
	{O5_GPIO_Port, O5_Pin, O11_GPIO_Port, O11_Pin},
	{O6_GPIO_Port, O6_Pin, O12_GPIO_Port, O12_Pin},
};

void Handler_picker(uint8_t id, uint16_t state)
{
    HandlerIO *io = &handler_io[id];

    switch(state)
    {
        case 1: // down
            HAL_GPIO_WritePin(io->down_port, io->down_pin, GPIO_PIN_RESET);
            break;

        case 2: // up
        	 HAL_GPIO_WritePin(io->down_port, io->down_pin, GPIO_PIN_SET);
          //  HAL_GPIO_WritePin(io->up_port, io->up_pin, GPIO_PIN_SET);
            break;

        case 4: // pick
            HAL_GPIO_WritePin(io->pick_port, io->pick_pin, GPIO_PIN_RESET);
            break;

        case 8: // RELEASE
           // HAL_GPIO_WritePin(io->release_port, io->release_pin, GPIO_PIN_SET);
        	 HAL_GPIO_WritePin(io->pick_port, io->pick_pin, GPIO_PIN_SET);
            break;
    }
}

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

void SetPickRubber(uint8_t vacum_id) // 0 .. 5
{
    if (vacum_id > 5) return;

    Handle_Pick[vacum_id].vacum_id   = vacum_id;
    Handle_Pick[vacum_id].retry      = 0;
    Handle_Pick[vacum_id].time_delay = 0;
    Handle_Pick[vacum_id].result     = RUNNING;
    Handle_Pick[vacum_id].state      = CYLINDER_GO_DOWN;
}

void SetReleaseRubber(uint8_t vacum_id)   // 0 .. 5
{
    if (vacum_id > 5) return;
    Handle_Release[vacum_id].vacum_id   = vacum_id;
    Handle_Release[vacum_id].retry      = 0;
    Handle_Release[vacum_id].time_delay = 0;
    Handle_Release[vacum_id].result     = RUNNING;
    Handle_Release[vacum_id].state      = CYLINDER_GO_DOWN;
}


void PickRubber1(uint8_t vacum_id)
{
	if (vacum_id > 5) return;
	switch (Handle_Pick[vacum_id].state)
    {
		case IDLE:
		{
            Handle_Release[vacum_id].retry = 0;
			break;
    	}
		case CYLINDER_GO_DOWN:
		{
            if (vacum_id == 0) 						Cylinder1_Go_Down;
            else if(vacum_id == 1)               	Cylinder2_Go_Down;
            else if(vacum_id == 2)               	Cylinder3_Go_Down;
            else if(vacum_id == 3)               	Cylinder4_Go_Down;
            else if(vacum_id == 4)               	Cylinder5_Go_Down;
            else if(vacum_id == 5)               	Cylinder6_Go_Down;

            if(Handle_Pick[vacum_id].retry == 0){
            	Handle_Pick[vacum_id].time_delay = TIME_DOWN;
            }
            else{
            	Handle_Pick[vacum_id].time_delay = TIME_DOWN * (Handle_Pick[vacum_id].retry + 1);
            }

            Handle_Pick[vacum_id].state = VACUM_REALSE_OFF;
			break;
		}
		case VACUM_REALSE_OFF:
		{
            if (Handle_Pick[vacum_id].time_delay == 0)
            {
                if (vacum_id == 0){
                	//Vacum1_Release_Off;
                	Vacum1_Pick_Off;
                }
                else if (vacum_id == 1){
                	//Vacum2_Release_Off;
                	Vacum2_Pick_Off;
                }
                else if (vacum_id == 2){
                	//Vacum3_Release_Off;
                	Vacum3_Pick_Off;
                }
                else if (vacum_id == 3){
                	//Vacum4_Release_Off;
                	Vacum4_Pick_Off;
                }
                else if (vacum_id == 4){
                	//Vacum5_Release_Off;
                	Vacum5_Pick_Off;
                }
                else if (vacum_id == 5){
                	//Vacum6_Release_Off;
                	Vacum6_Pick_Off;
                }

                Handle_Pick[vacum_id].time_delay = TIME_SWITCH_STATE;
                Handle_Pick[vacum_id].state = VACUM_PICK_ON;
            }
            break;
		}
		case VACUM_PICK_ON:
		{
            if (Handle_Pick[vacum_id].time_delay == 0)
            {
                if (vacum_id == 0) 					Vacum1_Pick_On;
                else if(vacum_id == 1)            	Vacum2_Pick_On;
                else if(vacum_id == 2)            	Vacum3_Pick_On;
                else if(vacum_id == 3)            	Vacum4_Pick_On;
                else if(vacum_id == 4)            	Vacum5_Pick_On;
                else if(vacum_id == 5)            	Vacum6_Pick_On;

                Handle_Pick[vacum_id].time_delay = TIME_AIR_UP;
                Handle_Pick[vacum_id].state = CYLINDER_GO_UP;
            }
            break;
		}
		case CYLINDER_GO_UP:
		{
            if (Handle_Pick[vacum_id].time_delay == 0)
            {
                if (vacum_id == 0) 					Cylinder1_Go_Up;
                else if (vacum_id == 1)             Cylinder2_Go_Up;
                else if (vacum_id == 2)             Cylinder3_Go_Up;
                else if (vacum_id == 3)             Cylinder4_Go_Up;
                else if (vacum_id == 4)             Cylinder5_Go_Up;
                else if (vacum_id == 5)             Cylinder6_Go_Up;

                if(Handle_Pick[vacum_id].retry == 0){
                	Handle_Pick[vacum_id].time_delay = TIME_UP;
                }
                else{
                	Handle_Pick[vacum_id].time_delay = TIME_UP * (Handle_Pick[vacum_id].retry + 1);
                }

                Handle_Pick[vacum_id].state = RETRY;
            }
            break;
		}
		case RETRY:
		{
            if (Handle_Pick[vacum_id].time_delay == 0)
            {
                uint8_t ok = 0;

//                if (vacum_id == 0)
//                    ok = Is_Vacum1_Pick;
//                else if (vacum_id == 1)
//                    ok = Is_Vacum2_Pick;
//                else if (vacum_id == 2)
//                    ok = Is_Vacum3_Pick;
//                else if (vacum_id == 3)
//                    ok = Is_Vacum4_Pick;
//                else if (vacum_id == 4)
//                    ok = Is_Vacum5_Pick;
//                else if (vacum_id == 5)
//                    ok = Is_Vacum6_Pick;

                if (!ok)
                    Handle_Pick[vacum_id].state = DONE_OK;
                else if (++Handle_Pick[vacum_id].retry < 2)
                    Handle_Pick[vacum_id].state = CYLINDER_GO_DOWN;
                else
                    Handle_Pick[vacum_id].state = DONE_NG;
            }
            break;
		}
		case DONE_OK:
		{
            Handle_Pick[vacum_id].result = OK;
            Handle_Pick[vacum_id].state  = IDLE;
            break;
		}
		case DONE_NG:
		{
            Handle_Pick[vacum_id].result = NG;
            Handle_Pick[vacum_id].state  = IDLE;
            break;
		}
    }
    if (Handle_Pick[vacum_id].time_delay > 0)
        Handle_Pick[vacum_id].time_delay--;
}

void ReleaseRubber1(uint8_t vacum_id)
{
	if (vacum_id > 5) return;
	switch (Handle_Release[vacum_id].state)
    {
		case IDLE:
		{
            Handle_Release[vacum_id].retry = 0;
			break;
    	}
		case CYLINDER_GO_DOWN:
		{
            if (vacum_id == 0) 						Cylinder1_Go_Down;
            else if(vacum_id == 1)               	Cylinder2_Go_Down;
            else if(vacum_id == 2)               	Cylinder3_Go_Down;
            else if(vacum_id == 3)               	Cylinder4_Go_Down;
            else if(vacum_id == 4)               	Cylinder5_Go_Down;
            else if(vacum_id == 5)               	Cylinder6_Go_Down;

            if(Handle_Release[vacum_id].retry == 0){
            	Handle_Release[vacum_id].time_delay = TIME_DOWN;
            }
            else{
            	Handle_Release[vacum_id].time_delay = TIME_DOWN * (Handle_Release[vacum_id].retry + 1);
            }

            Handle_Release[vacum_id].state = VACUM_PICK_OFF;
            break;
		}
		case VACUM_PICK_OFF:
		{
            if (Handle_Release[vacum_id].time_delay == 0)
            {
                if (vacum_id == 0){
                	Vacum1_Pick_Off;
                	//Vacum1_Release_Off;
                }
                else if (vacum_id == 1){
                	Vacum2_Pick_Off;
                	//Vacum2_Release_Off;
                }
                else if (vacum_id == 2){
                	Vacum3_Pick_Off;
                	//Vacum3_Release_Off;
                }
                else if (vacum_id == 3){
                	Vacum4_Pick_Off;
                	//Vacum4_Release_Off;
                }
                else if (vacum_id == 4){
                	Vacum5_Pick_Off;
                	//Vacum5_Release_Off;
                }
                else if (vacum_id == 5){
                	Vacum6_Pick_Off;
                	//Vacum6_Release_Off;
                }

                Handle_Release[vacum_id].time_delay = TIME_SWITCH_STATE;
                Handle_Release[vacum_id].state = VACUM_REALSE_ON;
            }
            break;
		}

		case VACUM_REALSE_ON:
		{
            if (Handle_Release[vacum_id].time_delay == 0)
            {
                //if (vacum_id == 0) Vacum1_Release_On;
                //else               Vacum2_Release_On;

                Handle_Release[vacum_id].time_delay = TIME_AIR_DOWN;
                Handle_Release[vacum_id].state = CYLINDER_GO_UP;
            }
            break;
		}
		case CYLINDER_GO_UP:
		{
            if (Handle_Release[vacum_id].time_delay == 0)
            {
                if (vacum_id == 0) 					Cylinder1_Go_Up;
                else if (vacum_id == 1)             Cylinder2_Go_Up;
                else if (vacum_id == 2)             Cylinder3_Go_Up;
                else if (vacum_id == 3)             Cylinder4_Go_Up;
                else if (vacum_id == 4)             Cylinder5_Go_Up;
                else if (vacum_id == 5)             Cylinder6_Go_Up;

                if(Handle_Release[vacum_id].retry == 0){
                	Handle_Release[vacum_id].time_delay = TIME_UP;
                }
                else{
                	Handle_Release[vacum_id].time_delay = TIME_UP * (Handle_Release[vacum_id].retry + 1);
                }

                Handle_Release[vacum_id].state = RETRY;
            }
            break;
		}

		case RETRY:
		{
            if (Handle_Release[vacum_id].time_delay == 0)
            {
                uint8_t still_pick = 0;

//                if (vacum_id == 0)
//                    still_pick = Is_Vacum1_Pick;
//                else if(vacum_id == 1)
//                    still_pick = Is_Vacum2_Pick;
//                else if(vacum_id == 2)
//                    still_pick = Is_Vacum3_Pick;
//                else if(vacum_id == 3)
//                    still_pick = Is_Vacum4_Pick;
//                else if(vacum_id == 4)
//                    still_pick = Is_Vacum5_Pick;
//                else if(vacum_id == 5)
//                    still_pick = Is_Vacum6_Pick;

                if (!still_pick)
                    Handle_Release[vacum_id].state = DONE_OK;
                else if (++Handle_Release[vacum_id].retry < 2)
                    Handle_Release[vacum_id].state = CYLINDER_GO_DOWN;
                else
                    Handle_Release[vacum_id].state = DONE_NG;
            }
            break;
		}
		case DONE_OK:
		{
            Handle_Release[vacum_id].result = OK;
            Handle_Release[vacum_id].state  = IDLE;
            break;
		}
		case DONE_NG:
		{
            Handle_Release[vacum_id].result = NG;
            Handle_Release[vacum_id].state  = IDLE;
            break;
		}
    }
    if (Handle_Release[vacum_id].time_delay > 0)
        Handle_Release[vacum_id].time_delay--;
}
