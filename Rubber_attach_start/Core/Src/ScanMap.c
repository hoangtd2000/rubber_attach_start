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

const Item* Rubber = Rubber_Tray;
const Item* Tray_1 = Tray1;
const Item* Tray_2 = Tray2;

extern Point2D Rubber_Mark[3];
extern Point2D Tray1_Mark[3];
extern Point2D Tray2_Mark[3];
volatile uint16_t Rubber_Index = 0;
uint16_t Tray_Index = 0;
uint8_t end_Cover = 25;
extern const uint32_t FlashStart;

extern uint16_t Input_Registers_Database[50];
extern uint32_t data[10];
extern Tab_main_t* Tab_main_indicator;

void Read_Tray_Data(){
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
	Tray1_Mark[0].raw = data[3];
	Tray1_Mark[1].raw = data[4];
	Tray1_Mark[2].raw = data[5];
	Mark[6] = Tray1_Mark[0].x;
	Mark[7] = Tray1_Mark[0].y;
	Mark[8] = Tray1_Mark[1].x;
	Mark[9] = Tray1_Mark[1].y;
	Mark[10] = Tray1_Mark[2].x;
	Mark[11] = Tray1_Mark[2].y;

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
	Calculate_Tray_Point(Rubber_Tray, Rubber_Mark, RUBBER_ROWS, RUBBER_COLS);
	Calculate_Tray_Point(Tray1, Tray1_Mark, TRAY_ROWS, TRAY_COLS);
	Calculate_Tray_Point(Tray2, Tray2_Mark, TRAY_ROWS, TRAY_COLS);
}

void Calculate_Tray_Point(Item* tray, const Point2D* point,uint8_t row, uint8_t col)
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


//void Handle(void){
//		ScreenMain->bits.START = 0;
//		Rubber_Index = 0;
//		uint8_t item_count = 16;
//		uint8_t count = 0;
//		while(count < item_count){
//			count++;
//			wait_handler_stop();
//			move_axis(Rubber[Rubber_Index].x, Rubber[Rubber_Index].y, max_z-4000);
//			Input_Registers_Database[0] =  Rubber_Index+1 ;
//			delay_us(500);
//			wait_handler_stop();
//			move_axis(Rubber[Rubber_Index].x, Rubber[Rubber_Index].y, max_z);
//			delay_us(1000);
////			wait_handler_stop();
////			move_axis(Tray_1[Rubber_Index].x, Tray_1[Rubber_Index].y, max_z-4000);
////			delay_us(500);
////			wait_handler_stop();
////			move_axis(Tray_1[Rubber_Index].x, Tray_1[Rubber_Index].y, max_z);
////			wait_handler_stop();
////			Input_Registers_Database[1] = Rubber_Index+1;
////			delay_us(1000);
//			Rubber_Index++;
//		}
//		wait_handler_stop();
//		move_axis(0, 0, 0);
//		Input_Registers_Database[0] =0 ;
//		Input_Registers_Database[1] = 0;
//}


#define PAIRS_PER_TRAY  4
#define MAX_TRAYS       2
#define MAX_PAIRS       (PAIRS_PER_TRAY * MAX_TRAYS)   // 24 cặp
#define RUBBER_TOTAL_PAIRS (RUBBER_COLS * (RUBBER_ROWS / 2))  // 100 cặp

uint16_t rubber_pair  = 0;   // đếm cặp trên khuôn cao su (0..99)
extern Tab_main_t* Tab_main;
void Handle(void){
	//ScreenMain->bits.START = 0;
	Tab_main->bits.start = 0;
	Tab_main_indicator->bits.start =  1 ;
	uint16_t tray_index   = 0;   // đếm số cặp đã bỏ vào tray (0..23)


	Clear_all_tray1();
	Clear_all_tray2();
	while(tray_index < MAX_PAIRS && rubber_pair < RUBBER_TOTAL_PAIRS) // Dừng khi đầy tray1,2 và hết hàng ở tray rubber
	{
		int rx = rubber_pair % RUBBER_COLS;
		int ry = (rubber_pair / RUBBER_COLS) * 2;

		// ===== Tray mapping =====
		int tray_id   = tray_index / PAIRS_PER_TRAY;
		int tray_pair = tray_index % PAIRS_PER_TRAY;

		int tx = tray_pair % TRAY_COLS;
		int ty = (tray_pair / TRAY_COLS) * 2;

		wait_handler_stop();
		move_axis(Rubber[ry * RUBBER_COLS + rx].x, Rubber[ry * RUBBER_COLS + rx].y, max_z - 4000);
		delay_us(1000);

		wait_handler_stop();
		move_axis(Rubber[ry * RUBBER_COLS + rx].x, Rubber[ry * RUBBER_COLS + rx].y, max_z);
		Clear_mark_rubber(ry * RUBBER_COLS + rx);

		Clear_mark_rubber(ry * RUBBER_COLS + rx +RUBBER_COLS );
		//Input_Registers_Database[0] =  Rubber_Index+1 ;

		delay_us(1000);
		if(tray_id == 0){
			wait_handler_stop();
			move_axis(Tray_1[ty * TRAY_COLS + tx].x, Tray_1[ty * TRAY_COLS + tx].y, max_z-4000);
			delay_us(500);
			wait_handler_stop();
			move_axis(Tray_1[ty * TRAY_COLS + tx].x, Tray_1[ty * TRAY_COLS + tx].y, max_z);
			wait_handler_stop();
			Mark_tray1(ty * TRAY_COLS + tx);
		//	Input_Registers_Database[1] = rubber_pair+1;
			delay_us(1000);

			wait_handler_stop();
			move_axis(Tray_1[(ty + 1) * TRAY_COLS + tx].x, Tray_1[(ty + 1) * TRAY_COLS + tx].y, max_z - 4000);
			delay_us(500);
			wait_handler_stop();
			move_axis(Tray_1[(ty + 1) * TRAY_COLS + tx].x, Tray_1[(ty + 1) * TRAY_COLS + tx].y, max_z);
			wait_handler_stop();
			Mark_tray1(ty * TRAY_COLS + tx + TRAY_COLS);
			//Input_Registers_Database[1] = tray_index+1;
			delay_us(1000);
		}
		if(tray_id == 1){
			wait_handler_stop();
			move_axis(Tray_2[ty * TRAY_COLS + tx].x, Tray_2[ty * TRAY_COLS + tx].y, max_z-4000);
			delay_us(500);
			wait_handler_stop();
			move_axis(Tray_2[ty * TRAY_COLS + tx].x, Tray_2[ty * TRAY_COLS + tx].y, max_z);
			wait_handler_stop();
			Mark_tray2(ty * TRAY_COLS + tx);
		//	Input_Registers_Database[1] = tray_index+1;
			delay_us(1000);

			wait_handler_stop();
			move_axis(Tray_2[(ty + 1) * TRAY_COLS + tx].x, Tray_2[(ty + 1) * TRAY_COLS + tx].y, max_z - 4000);
			delay_us(500);
			wait_handler_stop();
			move_axis(Tray_2[(ty + 1) * TRAY_COLS + tx].x, Tray_2[(ty + 1) * TRAY_COLS + tx].y, max_z);
			wait_handler_stop();
			Mark_tray2(ty * TRAY_COLS + tx + TRAY_COLS);
			//Input_Registers_Database[1] = tray_index+1;
			delay_us(1000);
		}
		rubber_pair++;    // quét tuần tự khuôn cao su
		tray_index++;     // đếm số đã bỏ vào tray
	}
	wait_handler_stop();
	move_axis(0, 0, 0);
//	Input_Registers_Database[0] =0 ;
//	Input_Registers_Database[1] = 0;
	Tab_main_indicator->bits.start =  0 ;
	if(rubber_pair >= RUBBER_TOTAL_PAIRS){
		rubber_pair  = 0;
		Mark_all_rubber();
	}
}



//#define RUBBER_COLS 8
//#define RUBBER_ROWS 7
//
//#define TRAY_COLS   4
//#define TRAY_ROWS   3
//
//#define PAIRS_PER_TRAY  12     // 24 con
//#define MAX_TRAYS       2
//#define MAX_PAIRS       (PAIRS_PER_TRAY * MAX_TRAYS)   // 24 lần gắp
//void Handle(uint8_t end_cover){
//	if(ScreenMain->bits.START == 1){
//		ScreenMain->bits.START = 0;
//		uint16_t Rubber_Index = 0;
//		while(Rubber_Index  < 12){
//            int rx = (Rubber_Index % RUBBER_COLS);
//            int ry = ((Rubber_Index / RUBBER_COLS) * 2);
//
//            int tray_id   = Rubber_Index / 12; // So luong tray1 và 2
//            int tray_pair = Rubber_Index  % PAIRS_PER_TRAY;
//
//            int tx = (tray_pair % TRAY_COLS);
//            int ty = ((tray_pair / TRAY_COLS) * 2);
//
//			wait_handler_stop();
//			move_axis(Rubber[ry * RUBBER_COLS + rx].x, Rubber[ry * RUBBER_COLS + rx].y, AxisZ.current_pos);
//			Input_Registers_Database[0] =  Rubber_Index+1 ;
//			delay_us(500);
//
//			if(tray_id == 0){
//				wait_handler_stop();
//				move_axis(Tray1[ty * TRAY_COLS + tx].x, Tray1[ty * TRAY_COLS + tx].y, AxisZ.current_pos);
//				Input_Registers_Database[0] =  Rubber_Index+1 ;
//				delay_us(500);
//
//				wait_handler_stop();
//				move_axis(Tray1[(ty + 1) * TRAY_COLS + tx].x, Tray1[(ty + 1) * TRAY_COLS + tx].y, AxisZ.current_pos);
//				Input_Registers_Database[0] =  Rubber_Index+1 ;
//				delay_us(500);
//			}
//
//			Rubber_Index++;
//		}
//		wait_handler_stop();
//		move_axis(0, 0, 0);
//		Input_Registers_Database[0] =0 ;
//		Input_Registers_Database[1] = 0;
//
//	}
//}

//void Handle(uint8_t end_cover){
//	if(ScreenMain->bits.START == 1){
//		ScreenMain->bits.START = 0;
//
//		Rubber_Index = 0;
//		uint8_t item_count = end_cover;
//		uint8_t count = 0;
//		while(count < item_count){
//
//			count++;
//			// Data Flash
//			wait_handler_stop();
//			move_axis(Rubber[Rubber_Index].x, Rubber[Rubber_Index].y, AxisZ.current_pos);
//		//	wait_handler_stop();
//			delay_us(2000);
//				Input_Registers_Database[0] =  Rubber_Index+1 ;
//			Rubber_Index++;
//		}
//	}
//}



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
