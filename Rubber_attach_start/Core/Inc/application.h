/*
 * application.h
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_
#include "modbusRTU.h"
#include "motor_control.h"
#include "stm32f4xx_ll_tim.h"
#include "ScanMap.h"
#include "flash.h"
#include "common.h"
#include "IO_Controller.h"

void application_init(void);
void application_run_main(void);
void task_timer6(void);
void task_timer7(void);

void Try_go_home();

void Handle_main(void);
void Handle_motor(void);
void Handle_setting(void);

void Handle_reset(void);
void Handle_start(void);
void Handle_stop(void);
void Handle_empty(void);

void Popup_handle_next(void);
void Popup_handle_stop(void);


#define Mark_all_rubber()  		(SetBit(&Inputs_Database[3], 0, 200))

#define Mark_rubber(f) 			(SetBit(&Inputs_Database[3], f, 1))
#define Clear_mark_rubber(f) 	(ClearBit(&Inputs_Database[3], f, 1))


#define Clear_all_tray1() 	(ClearBit(&Inputs_Database[28], 0, 24))
#define Mark_tray1(f) 			(SetBit(&Inputs_Database[28], f, 1))
#define Clear_mark_tray1(f) 	(ClearBit(&Inputs_Database[28], f, 1))

#define Clear_all_tray2() 	(ClearBit(&Inputs_Database[31], 0, 24))
#define Mark_tray2(f) 			(SetBit(&Inputs_Database[31], f, 1))
#define Clear_mark_tray2(f) 	(ClearBit(&Inputs_Database[31], f, 1))

#define Mark_rubber_working(f)  Input_Registers_Database[0] =  f+1

#define Close_Popup(f) 		(ClearBit(&Inputs_Database[34], f, 1))
#define Open_Popup(f) 			(SetBit(&Inputs_Database[34], f, 1))
#define Read_Warring(f)        (ReadBit(&Inputs_Database[34], f, 1))

extern uint8_t SS_Door_Left;
extern uint8_t SS_Door_Right;

#endif /* INC_APPLICATION_H_ */
