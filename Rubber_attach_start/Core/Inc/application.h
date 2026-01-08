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

void application_init(void);
void application_run(void);
void task_timer6(void);
typedef void (*ActionHandler_t)(void);


#endif /* INC_APPLICATION_H_ */
