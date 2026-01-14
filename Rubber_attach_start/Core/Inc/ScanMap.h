/*
 * ScanMap.h
 *
 *  Created on: Jan 10, 2026
 *      Author: mcnex
 */

#ifndef INC_SCANMAP_H_
#define INC_SCANMAP_H_

#include "tick.h"
#include "main.h"
#include "motor_control.h"
#include "application.h"
// Vi tri rubber
#define X_OFFSET 60
#define Y_OFFSET 150
#define Rubber_Via 350

//#define Is_Vacum1_Pick		HAL_GPIO_ReadPin(I1_GPIO_Port, I2_Pin)
//#define Is_Vacum2_Pick		HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin)

#define Cylinder1_Go_Down 	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, 0)
#define Cylinder1_Go_Up 	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, 1)
#define Cylinder2_Go_Down 	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, 0)
#define Cylinder2_Go_Up 	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, 1)

#define Vacum1_Pick			HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, 1)
							//HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, 1)
#define Vacum1_Release		HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, 1)
							//HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, 1)
#define Vacum2_Pick			HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, 1)
							//HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, 1)
#define Vacum2_Release		HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, 1)
							//HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, 1)
//#define Is_Vacum1_Pick		HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin)
//#define Is_Vacum2_Pick		HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin)


#define RUBBER_COLS 20
#define RUBBER_ROWS 4 // luôn là số chẵn

#define TRAY_COLS   4
#define TRAY_ROWS   4

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

void Calculate_Tray_Point(Item* tray, const Point2D* point,uint8_t row, uint8_t col);
uint8_t Pick_Item(uint8_t No);

#endif /* INC_SCANMAP_H_ */
