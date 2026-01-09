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
void application_run_main(void);
void task_timer6(void);
typedef void (*ActionHandler_t)(void);

typedef union {
    struct {

        uint8_t home	: 1;
        uint8_t motor	: 1;
        uint8_t setting		: 1;
    } bits;
    uint8_t all;
} control_taskbar_t;



#endif /* INC_APPLICATION_H_ */
