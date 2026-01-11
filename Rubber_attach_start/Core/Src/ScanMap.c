/*
 * ScanMap.c
 *
 *  Created on: Jan 10, 2026
 *      Author: mcnex
 */

#include "ScanMap.h"
#include "stdbool.h"

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
TrayPos Tray1Pos_Write 		 = {.row = 6,.col = 4};
TrayPos Tray2Pos_Write 		 = {.row = 6,.col = 4};

const Item* Rubber = Rubber_Tray;
const Item* Tray_1 = Tray1;
const Item* Tray_2 = Tray2;

extern Point2D Rubber_Mark[3];
extern Point2D Tray1_Mark[3];
extern Point2D Tray2_Mark[3];
uint16_t Rubber_Index = 0;
uint8_t end_Cover = 25;
extern const uint32_t FlashStart;

extern uint16_t Input_Registers_Database[50];
extern uint32_t data[10];
static inline bool AllAxisStop(void)
{
    return (AxisX.mode == STOP &&
            AxisY.mode == STOP &&
            AxisZ.mode == STOP);
}

void wait_handler_stop(){
	while (!AllAxisStop())
	{

	}
}

void Read_Tray_Data(){
	//uint32_t data[10];
	Flash_Read_Data( FlashStart, data, 10);

	Rubber_Mark[0].raw = data[0];
	Rubber_Mark[1].raw = data[1];
	Rubber_Mark[2].raw = data[2];
	Mark[0] = Rubber_Mark[0].x;
	Mark[1] = Rubber_Mark[0].y;
	Mark[2] = Rubber_Mark[1].x;
	Mark[3] = Rubber_Mark[1].y;
	Mark[4] = Rubber_Mark[2].x;
	Mark[5] = Rubber_Mark[2].y;

//	Calculate_TrayRubber_Point(Rubber_Tray, Rubber_Mark, Rubber_TrayPos.row, Rubber_TrayPos.col);
	Calculate_TrayRubber_Point(Rubber_Tray, Rubber_Mark,10, 20);


	Tray1_Mark[0].raw = data[3];
	Tray1_Mark[1].raw = data[4];
	Tray1_Mark[2].raw = data[5];
	Mark[6] = Tray1_Mark[0].x;
	Mark[7] = Tray1_Mark[0].y;
	Mark[8] = Tray1_Mark[1].x;
	Mark[9] = Tray1_Mark[1].y;
	Mark[10] = Tray1_Mark[2].x;
	Mark[11] = Tray1_Mark[2].y;

//	Calculate_Tray1_Point(Tray1, Tray1_Mark, Tray1Pos.row, Tray1Pos.col);
	Calculate_Tray1_Point(Tray1, Tray1_Mark, 6, 4);

	Tray2_Mark[0].raw = data[6];
	Tray2_Mark[1].raw = data[7];
	Tray2_Mark[2].raw = data[8];
	Mark[12] = Tray2_Mark[0].x;
	Mark[13] = Tray2_Mark[0].y;
	Mark[14] = Tray2_Mark[1].x;
	Mark[15] = Tray2_Mark[1].y;
	Mark[16] = Tray2_Mark[2].x;
	Mark[17] = Tray2_Mark[2].y;
//	Calculate_Tray2_Point(Tray2, Tray2_Mark, Tray2Pos.row, Tray2Pos.col);
	Calculate_Tray2_Point(Tray2, Tray2_Mark, 6, 4);
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

		Rubber_Index = 0;
	//	Handle_tray_rubber_p1();
		uint8_t item_count = end_cover;
		uint8_t count = 0;
		while(count < item_count){

			count++;
			// Data Flash
			wait_handler_stop();
			move_axis(Rubber[Rubber_Index].x, Rubber[Rubber_Index].y, AxisZ.current_pos);
	//		HAL_Delay(200);

				Input_Registers_Database[0] =  Rubber_Index+1 ;
//			if(Pick_Item(1) == 1){	// pick item 1
//				HAL_Delay(200);
//				//SetBit(Lamp_glass_empty, Glass_Index % 196, 1); // set lamp glass
//				//SetStateTrayRubber(Empty, Glass_Index);				// set TrayRubber state
//				Rubber_Index++;									        // increase tray rubber index
//			}
//			else{
//				//Red_on;
//				return;
//			}
			//Pick_Item(2);
			wait_handler_stop();
			move_axis(Tray_1[Rubber_Index].x, Tray_1[Rubber_Index].y, AxisZ.current_pos);
			Input_Registers_Database[1] =  Rubber_Index + 1;
		//	HAL_Delay(200);
			Rubber_Index++;
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
