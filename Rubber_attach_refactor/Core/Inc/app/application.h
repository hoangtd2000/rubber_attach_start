/*
 * application.h
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_
#include "modbus_rtu.h"
#include "motor_control.h"
#include "stm32f4xx_ll_tim.h"
#include "ScanMap.h"
#include "flash.h"
#include "common.h"
#include "IO_Controller.h"
#include "hmi_control.h"

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




//#define Close_Popup_home() (ClearBit(&Inputs_Database[34], 2, 1))
//#define Open_Popup_home() 	(SetBit(&Inputs_Database[34], 2, 1))

#endif /* INC_APPLICATION_H_ */
