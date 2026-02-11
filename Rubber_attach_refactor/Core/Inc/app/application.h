/*
 * application.h
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_
#include "dio_control.h"
#include "modbus_rtu.h"
#include "motor_control.h"
#include "stm32f4xx_ll_tim.h"
#include "flash.h"
#include "common.h"
#include "hmi_control.h"
#include "tick.h"

#define RUBBER_COLS 20
#define RUBBER_ROWS 10 // luôn là số chẵn

#define TRAY_COLS   4
#define TRAY_ROWS   6

#define PAIRS_PER_TRAY  12
#define MAX_TRAYS       2
#define MAX_PAIRS       (PAIRS_PER_TRAY * MAX_TRAYS)   // 24 cặp
#define RUBBER_TOTAL_PAIRS (RUBBER_COLS * (RUBBER_ROWS / 2))  // 100 cặp


//#define Y_Calibrator 2600U  //2500
//#define X_Calibrator 0U     //2500

#define Y_Calibrator 0U  //2500
#define X_Calibrator 2600U     //2500

#define TIME_DOWN 200U
#define TIME_UP   200U
#define TIME_AIR_UP    200U
#define TIME_AIR_DOWN  200U
#define TIME_SWITCH_STATE 40U

typedef enum {
    ST_IDLE,
    ST_MOVE_TO_RUBBER,
    ST_PICK1,
	ST_WAIT_PICK1,
	ST_PICK2,
	ST_WAIT_PICK2,
    ST_WAIT_POPUP,
	ST_CONTINUE,
    ST_PLACE1,
	ST_PLACE2,
	ST_RELEASE1,
	ST_WAIT_RELEASE1,
	ST_RELEASE2,
	ST_WAIT_RELEASE2,
    ST_NEXT_PAIR,
    ST_STOP,
	ST_PAUSE,
	ST_PAUSE_DOOR,
	ST_STOP_PULSE,
} PickState_t;

typedef union {
    struct {
        uint16_t x;
        uint16_t y;
    };
    uint32_t raw;
} Point2D;

void Handle(void);
void wait_handler_stop();
void Read_Tray_Data();

void Calculate_Tray_Point(Point2D* tray, const Point2D* point, uint8_t row, uint8_t col);
void Calculate_TrayRubber_Point(Point2D* tray, const Point2D* point,uint8_t row, uint8_t col);
void PlaceToTray(Point2D *tray, uint8_t tray_id, int index);


void application_init(void);
void application_run_main(void);
void task_timer6(void);
void task_timer7(void);

void Try_go_home();





#endif /* INC_APPLICATION_H_ */
