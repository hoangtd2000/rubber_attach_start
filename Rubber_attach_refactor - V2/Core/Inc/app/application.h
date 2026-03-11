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
#define RUBBER_ROWS 10
#define TOTAL_SLOTS_RUBBER  (RUBBER_COLS * RUBBER_ROWS)  // 200 item

#define TRAY_COLS   4
#define TRAY_ROWS   6

#define GRIP_COL 2
#define GRIP_ROW 3

#define ITEMS_PER_PICK      6      // mỗi lần gắp/đặt = 6 con
#define SLOTS_PER_TRAY      4      // 24 con / 6 = 4 slot
#define MAX_TRAYS           2
#define TOTAL_SLOTS         (SLOTS_PER_TRAY * MAX_TRAYS)  // 8 slot

#define Y_Calibrator 0U  //2500
#define X_Calibrator 0U     //2500

#define TIME_DOWN 200U
#define TIME_UP   200U
#define TIME_AIR_UP    200U
#define TIME_AIR_DOWN  200U
#define TIME_SWITCH_STATE 40U

typedef struct {
    int dx;
    int dy;
} GripOffset_t;

extern GripOffset_t GripOffset[ITEMS_PER_PICK];

typedef enum {
    ST_IDLE,

	ST_PICK_MOVE,
    ST_PICK,
    ST_WAIT_PICK,

	ST_CHECK_GRIP,

	ST_CHECK_FULL,

    /* ==== PLACE ==== */
    ST_MOVE_TO_TRAY,
    ST_RELEASE_ALL,
    ST_WAIT_RELEASE,

    ST_NEXT_SLOT,
    ST_STOP,
    ST_PAUSE,
} PickState_t;

typedef union {
    struct {
        uint16_t x;
        uint16_t y;
        uint16_t z;
        uint16_t reserve;
    };
    uint64_t raw;
} Point3D;

void Handle(void);
void wait_handler_stop();
void Read_Tray_Data();

void PrepareBlock(int slot);
void Calculate_Tray_Point(Point3D* tray, const Point3D* point, uint8_t row, uint8_t col);
void Calculate_TrayRubber_Point(Point3D* tray, const Point3D* point,uint8_t row, uint8_t col);
void MoveToTray(Point3D *tray, uint8_t tray_id, int index);
//void MoveToRubber(int index);
void MoveToRubber(int rubber_index, int grip_id);
void CalcBlockBase(int slot, int *base_rx, int *base_ry);

void Read_Index_Rubber();
void Read_Index_Tray();
void Set_Possition();

void application_init(void);
void application_run_main(void);
void task_timer6(void);
void task_timer7(void);

void Try_go_home();

#endif /* INC_APPLICATION_H_ */
