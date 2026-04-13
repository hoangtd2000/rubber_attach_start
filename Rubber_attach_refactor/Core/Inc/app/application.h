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
#define RUBBER_TOTAL 200
#define TRAY_COLS   4
#define TRAY_ROWS   6

#define MODEL_DATA_WORDS   28   // 1 row/col + 9*3 (Rubber + Tray1 + Tray2)
#define MODEL_META_WORDS   1
#define MAX_MODELS         4    // số model có thể lưu trong flash
#define FLASH_MODEL_WORDS  (MODEL_META_WORDS + MAX_MODELS * MODEL_DATA_WORDS)

#define PAIRS_PER_TRAY  12

typedef struct {
    uint8_t modelIndex;
    uint8_t rows;
    uint8_t cols;
} ModelConfig_t;

extern ModelConfig_t ModelConfigs[MAX_MODELS];
#define MAX_TRAYS       2
#define MAX_PAIRS       (PAIRS_PER_TRAY * MAX_TRAYS)   // 24 cặp
#define RUBBER_TOTAL_PAIRS (RUBBER_COLS * (RUBBER_ROWS / 2))  // 100 cặp


//#define Y_Calibrator 2600U  //2500
//#define X_Calibrator 0U     //2500
//#define Z_Calibrator 0U

#define X_Calibrator 0U  //2500
#define Y_Calibrator 2600U     //2500
#define Z_Calibrator 0U

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
        uint16_t z;
    };
    uint64_t raw;
} Point3D;

void Handle(void);
void wait_handler_stop();
void CopyMarkToArray(uint16_t *dst, Point3D *src, uint8_t count);
void Read_Tray_Data();

void Calculate_Tray_Point(Point3D* tray, const Point3D* point, uint8_t row, uint8_t col);
void Calculate_TrayRubber_Point(Point3D* tray, const Point3D* point,uint8_t row, uint8_t col);
void PlaceToTray(Point3D *tray, uint8_t tray_id, int index);

uint8_t GetCurrentModelRows(void);
uint8_t GetCurrentModelCols(void);


void application_init(void);
void application_run_main(void);
void task_timer6(void);
void task_timer7(void);

void Try_go_home();





#endif /* INC_APPLICATION_H_ */
