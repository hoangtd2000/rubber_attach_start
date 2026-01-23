/*
 * ScanMap.h
 *
 *  Created on: Jan 10, 2026
 *      Author: CongChuc
 */

#ifndef INC_SCANMAP_H_
#define INC_SCANMAP_H_

#include "tick.h"
#include "main.h"
#include "motor_control.h"
#include "application.h"

#define RUBBER_COLS 20
#define RUBBER_ROWS 10 // luôn là số chẵn

#define TRAY_COLS   4
#define TRAY_ROWS   6

#define Y_Calibrator 0U  //2500
#define X_Calibrator 0U     //2500

// Vị trí từng Rubber
typedef enum {
	Empty,
	Not_Empty,
} ItemState;

typedef struct {
	uint16_t x;
	uint16_t y;
	ItemState State;
}Item;

typedef enum {
    ST_IDLE,
    ST_MOVE_TO_RUBBER,
    ST_PICK1,
	ST_WAIT_PICK1,
	ST_PICK2,
	ST_WAIT_PICK2,
    ST_WAIT_POPUP,
    ST_PLACE1,
	ST_PLACE2,
	ST_RELEASE1,
	ST_WAIT_RELEASE1,
	ST_RELEASE2,
	ST_WAIT_RELEASE2,
    ST_NEXT_PAIR,
    ST_STOP,
	ST_PAUSE_DOOR,
	ST_STOP_PULSE,
} PickState_t;


void Handle(void);
void wait_handler_stop();
void Read_Tray_Data();

void Calculate_Tray_Point(Item* tray, const Point2D* point, uint8_t row, uint8_t col);
void Calculate_TrayRubber_Point(Item* tray, const Point2D* point,uint8_t row, uint8_t col);
void PlaceToTray(Item *tray, uint8_t tray_id, int index);
void CheckDoorAndPause(void);
#endif /* INC_SCANMAP_H_ */
