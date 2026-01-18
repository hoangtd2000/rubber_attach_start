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

void Handle(void);
void wait_handler_stop();
void Read_Tray_Data();

void Calculate_Tray_Point(Item* tray, const Point2D* point, uint8_t row, uint8_t col);
void PlaceToTray(Item *tray, uint8_t tray_id, int index);
#endif /* INC_SCANMAP_H_ */
