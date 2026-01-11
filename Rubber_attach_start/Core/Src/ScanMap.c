/*
 * ScanMap.c
 *
 *  Created on: Jan 10, 2026
 *      Author: mcnex
 */

#include "ScanMap.h"


// Screen Main
extern Axis_t AxisX, AxisY, AxisZ;
extern uint16_t* Mark;
extern ScreenMain_t* ScreenMain;

Item Rubber_Tray[400] = { [0 ... 399] = { .State = Not_Empty } };;
Item Tray1[30];
Item Tray2[30];

TrayPos Rubber_TrayPos;
TrayPos Tray1Pos;
TrayPos Tray2Pos;

// Data write to Flash
TrayPos Rubber_TrayPos_Write = {.row = 10,.col = 20};
TrayPos Tray1Pos_Write 		 = {.row = 4,.col = 6};
TrayPos Tray2Pos_Write 		 = {.row = 4,.col = 6};

const Item* Rubber = Rubber_Tray;
const Item* Tray_1 = Tray1;
const Item* Tray_2 = Tray2;

void Read_Tray_Data(){
	Point2D Rubber_Mark[3];
	uint32_t data[8];
	//Flash_Read_Data( FlashStart, data, 8);
	if((data[0] + data[1] + data[2] + data[3]) == 0
			|| data[0] == 0xffffffff || data[1] == 0xffffffff || data[2] == 0xffffffff || data[3] == 0xffffffff) return;
	if((data[4] + data [5] + data[6] + data[7]) == 0
			|| data[4] == 0xffffffff || data[5] == 0xffffffff || data[6] == 0xffffffff || data[7] == 0xffffffff) return;
	Rubber_Mark[0].raw = data[0];
	Rubber_Mark[1].raw = data[1];
	Rubber_Mark[2].raw = data[2];
	Rubber_TrayPos.raw = data[3];
	*(Mark) = Rubber_Mark[0].x;
	*(Mark + 1) = Rubber_Mark[0].y;
	*(Mark + 2) = Rubber_Mark[1].x;
	*(Mark + 3) = Rubber_Mark[1].y;
	*(Mark + 4) = Rubber_Mark[2].x;
	*(Mark + 5) = Rubber_Mark[2].y;
	Calculate_TrayRubber_Point(Rubber_Tray, Rubber_Mark, Rubber_TrayPos.row, Rubber_TrayPos.col);

	Point2D Tray1_Mark[3];

	//Flash_Read_Data( FlashStart+4, data, 3);
	Tray1_Mark[0].raw = data[4];
	Tray1_Mark[1].raw = data[5];
	Tray1_Mark[2].raw = data[6];
	Tray1Pos.raw = data[7];
	*(Mark + 6) = Tray1_Mark[0].x;
	*(Mark + 7) = Tray1_Mark[0].y;
	*(Mark + 8) = Tray1_Mark[1].x;
	*(Mark + 9) = Tray1_Mark[1].y;
	*(Mark + 10) = Tray1_Mark[2].x;
	*(Mark + 11) = Tray1_Mark[2].y;
	Calculate_Tray1_Point(Tray1, Tray1_Mark, Tray1Pos.row, Tray1Pos.col);

	Point2D Tray2_Mark[3];
	//Flash_Read_Data( FlashStart+4, data, 3);
	Tray2_Mark[0].raw = data[4];
	Tray2_Mark[1].raw = data[5];
	Tray2_Mark[2].raw = data[6];
	Tray2Pos.raw = data[7];
	*(Mark + 6) = Tray2_Mark[0].x;
	*(Mark + 7) = Tray2_Mark[0].y;
	*(Mark + 8) = Tray2_Mark[1].x;
	*(Mark + 9) = Tray2_Mark[1].y;
	*(Mark + 10) = Tray2_Mark[2].x;
	*(Mark + 11) = Tray2_Mark[2].y;
	Calculate_Tray2_Point(Tray2, Tray2_Mark, Tray2Pos.row, Tray2Pos.col);
}

void Calculate_TrayRubber_Point(Item* tray, const Point2D* point,
                                uint8_t row, uint8_t col)
{
    if (row < 2 || col < 2) return;          // tránh chia 0

    const double dx_row = (double)(point[2].x - point[0].x) / (row - 1);
    const double dy_row = (double)(point[2].y - point[0].y) / (row - 1);

    const double dx_col = (double)(point[1].x - point[0].x) / (col - 1);
    const double dy_col = (double)(point[1].y - point[0].y) / (col - 1);

    uint16_t index = 0;

    for (uint8_t i = 0; i < row; ++i) {
        for (uint8_t j = 0; j < col; ++j) {
            tray[index].x = point[0].x + i * dx_row + j * dx_col;
            tray[index].y = point[0].y + i * dy_row + j * dy_col;
            ++index;
        }
    }
}
void Calculate_Tray1_Point(Item* tray, const Point2D* point,
                           uint8_t row, uint8_t col)
{
    if (!tray || !point) return;
    if (row < 2 || col < 2) return;   // tránh chia 0

    const double dx_row = (double)(point[2].x - point[0].x) / (row - 1);
    const double dy_row = (double)(point[2].y - point[0].y) / (row - 1);

    const double dx_col = (double)(point[1].x - point[0].x) / (col - 1);
    const double dy_col = (double)(point[1].y - point[0].y) / (col - 1);

    uint16_t index = 0;

    for (uint8_t i = 0; i < row; ++i) {
        for (uint8_t j = 0; j < col; ++j) {
            tray[index].x = point[0].x + i * dx_row + j * dx_col;
            tray[index].y = point[0].y + i * dy_row + j * dy_col;
            ++index;
        }
    }
}

void Calculate_Tray2_Point(Item* tray, const Point2D* point,
                           uint8_t row, uint8_t col)
{
    if (!tray || !point) return;
    if (row < 2 || col < 2) return;  // tránh chia 0

    const double dx_row = (double)(point[2].x - point[0].x) / (row - 1);
    const double dy_row = (double)(point[2].y - point[0].y) / (row - 1);
    const double dx_col = (double)(point[1].x - point[0].x) / (col - 1);
    const double dy_col = (double)(point[1].y - point[0].y) / (col - 1);

    uint16_t index = 0; // tránh overflow uint16_t nếu row*col lớn

    for (uint8_t i = 0; i < row; ++i) {
        for (uint8_t j = 0; j < col; ++j) {
            tray[index].x = point[0].x + i * dx_row + j * dx_col;
            tray[index].y = point[0].y + i * dy_row + j * dy_col;
            ++index;
        }
    }
}

void Handle(uint8_t end_cover){
	if(ScreenMain->bits.START == 1){
		ScreenMain->bits.START = 0;
		Handle_tray_rubber_p1();
		uint8_t item_count = end_cover;
		uint8_t count = 0;
		while(count < item_count){

			count++;
			// Data Flash
			move_axis(Rubber[Rubber_Index].x, Rubber[Rubber_Index].y, AxisZ.current_pos);
			HAL_Delay(200);
			if(Pick_Item(1) == 1){	// pick item 1
				HAL_Delay(200);
				//SetBit(Lamp_glass_empty, Glass_Index % 196, 1); // set lamp glass
				//SetStateTrayRubber(Empty, Glass_Index);				// set TrayRubber state
				Rubber_Index++;									        // increase tray rubber index
			}
			else{
				//Red_on;
				return;
			}
			//Pick_Item(2);
			move_axis(Tray_1[Rubber_Index].x, Rubber[Rubber_Index].y, AxisZ.current_pos);
			HAL_Delay(200);

		}
	}
}

uint8_t Pick_Item(uint8_t No){
	if(No == 1){
		for(uint8_t i = 0; i < 3; i++){
			Cylinder1_Go_Down;
			HAL_Delay(500);
			Vacum1_Pick;
			HAL_Delay(500);
			Cylinder1_Go_Up;
//			if(Is_Vacum1_Pick == 0){
//				return 1;
//			}
		}
		return 0;
	}
	else if(No == 2){
		for(uint8_t i = 0; i < 3; i++){
			Cylinder2_Go_Down;
			HAL_Delay(500);
			Vacum2_Pick;
			HAL_Delay(500);
			Cylinder2_Go_Up;
//			if(Is_Vacum2_Pick == 0){
//				return 1;
//			}
		}
		return 0;
	}
	return 0;
}
