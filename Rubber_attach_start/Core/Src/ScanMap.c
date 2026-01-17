/*
 * ScanMap.c
 *
 *  Created on: Jan 10, 2026
 *      Author: CongChuc
 */

#include "ScanMap.h"

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

extern const uint32_t FlashStart;

extern uint16_t Input_Registers_Database[50];
extern uint32_t data[10];
extern Tab_main_t* Tab_main_indicator;
extern Tab_popup_t* Tab_popup;

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

#define PAIRS_PER_TRAY  12
#define MAX_TRAYS       2
#define MAX_PAIRS       (PAIRS_PER_TRAY * MAX_TRAYS)   // 24 cặp
#define RUBBER_TOTAL_PAIRS (RUBBER_COLS * (RUBBER_ROWS / 2))  // 100 cặp

typedef enum {
    ST_IDLE,
    ST_MOVE_TO_RUBBER,
    ST_PICK,
    ST_WAIT_POPUP,
    ST_PLACE,
    ST_NEXT_PAIR,
    ST_STOP
} PickState_t;

PickState_t pick_state = ST_IDLE;

uint16_t rubber_pair  = 0;   // đếm cặp trên khuôn cao su (0..99)
uint16_t tray_index   = 0;   // đếm số cặp đã bỏ vào tray (0..23)
Item *TrayList[] = { Tray1, Tray2 };
extern Tab_main_t* Tab_main;
uint8_t count_tray[10] = {0, 0};

#if 1

void Handle(void)
{
	Tab_main->bits.start = 0;
	Tab_main_indicator->bits.start =  1 ;
	Input_Registers_Database[3] = count_tray[0];
	Input_Registers_Database[4] = count_tray[1];
	if(tray_index == 0){
		Clear_all_tray1();
		Clear_all_tray2();
	}
	if(rubber_pair == 0){
		Mark_all_rubber();
	}
	while(tray_index < MAX_PAIRS && rubber_pair < RUBBER_TOTAL_PAIRS) // Dừng khi đầy tray1,2 và hết hàng ở tray rubber
    {
		switch(pick_state)
		{
			case ST_IDLE:
				Tab_main_indicator->bits.start =  1 ;
				Close_Popup(0);
				pick_state = ST_MOVE_TO_RUBBER;
			break;
			case ST_MOVE_TO_RUBBER:
			{
				OFF_LED_RED;
				ON_LED_GREEN;
				Tab_main_indicator->bits.start =  1 ;
				if(tray_index >= MAX_PAIRS || rubber_pair >= RUBBER_TOTAL_PAIRS)
				{
					pick_state = ST_STOP;
					break;
				}

				int pair_row = rubber_pair / RUBBER_COLS;
				int pair_col = rubber_pair % RUBBER_COLS;

				int rx = (pair_row & 1) ? (RUBBER_COLS - 1 - pair_col) : pair_col;
				int ry = pair_row * 2;

				// ===== Pick Rubber =====
				wait_handler_stop();
				move_axis(Rubber[ry * RUBBER_COLS + rx].x, Rubber[ry * RUBBER_COLS + rx].y, max_z_rubber - 8000);
				delay_us(1000);

				wait_handler_stop();
				move_axis(Rubber[ry * RUBBER_COLS + rx].x, Rubber[ry * RUBBER_COLS + rx].y, max_z_rubber);
				Clear_mark_rubber(ry * RUBBER_COLS + rx);
				Clear_mark_rubber(ry * RUBBER_COLS + rx + RUBBER_COLS );
				delay_us(1000);
				pick_state = ST_PICK;
			}
			break;
			case ST_PICK:
			{
				//!PickRubber(1) || !PickRubber(2)
				if(rubber_pair % 180 == 0)
				{
					wait_handler_stop();
					Open_Popup(0);
					SetBips(3);
					pick_state = ST_WAIT_POPUP;
				}
				else
				{
					pick_state = ST_PLACE;
				}
			}
			break;
			case ST_WAIT_POPUP:
				if(Timer_Check(1, 500) && Inputs_Database[34]){
					TOGGLE_LED_RED;
					OFF_LED_GREEN;
				}
				if(Tab_popup->bits.stop ==  1)
				{
					Tab_popup->bits.stop = 0;
					Close_Popup(0);
					rubber_pair++;   // bỏ cả cặp lỗi
					pick_state = ST_STOP;
				}
				if(Tab_popup->bits.next ==  1)
				{
					Tab_popup->bits.next = 0;
					Close_Popup(0);

					//ReleaseRubber(1);
					//ReleaseRubber(2);

					rubber_pair++;   // bỏ cả cặp lỗi
					pick_state = ST_MOVE_TO_RUBBER;
				}
			break;
			case ST_PLACE:
			{
				uint8_t tray_id   = tray_index / PAIRS_PER_TRAY;
				uint8_t tray_pair = tray_index % PAIRS_PER_TRAY;

				int tx = tray_pair % TRAY_COLS;
				int ty = (tray_pair / TRAY_COLS) * 2;

				Item *tray = TrayList[tray_id];

				PlaceToTray(tray, tray_id, ty * TRAY_COLS + tx);
				PlaceToTray(tray, tray_id, (ty + 1) * TRAY_COLS + tx);

				pick_state = ST_NEXT_PAIR;
			}
			break;
			case ST_NEXT_PAIR:
				rubber_pair++;
				tray_index++;
				pick_state = ST_MOVE_TO_RUBBER;
				break;
			case ST_STOP:
				wait_handler_stop();
				move_axis(0, 0, 0);
				wait_handler_stop();
				Tab_main_indicator->bits.start = 0;

				if(Tab_main->bits.start == 1){
					Tab_main->bits.start = 0;
					pick_state = ST_MOVE_TO_RUBBER;
				}
				break;
		}
    }
	SetBips(5);
	ON_LED_GREEN;
	wait_handler_stop();
	move_axis(0, 0, 0);
	wait_handler_stop();
	Tab_main_indicator->bits.start =  0 ;
	if(rubber_pair >= RUBBER_TOTAL_PAIRS){
		rubber_pair  = 0;
	}
	if(tray_index >= MAX_PAIRS){
		tray_index  = 0;
		count_tray[0] = 0;
		count_tray[1] = 0;
	}
}

void PlaceToTray(Item *tray, uint8_t tray_id, int index)
{
    wait_handler_stop();
    move_axis(tray[index].x, tray[index].y, max_z_tray - 8000);
    delay_us(500);
    wait_handler_stop();

    count_tray[tray_id]++;

    if(tray_id == 0){
        Mark_tray1(index);
        Input_Registers_Database[3] = count_tray[0];
    } else {
        Mark_tray2(index);
        Input_Registers_Database[4] = count_tray[1];
    }

    move_axis(tray[index].x, tray[index].y, max_z_tray);
    wait_handler_stop();
    delay_us(1000);
}
#else

int Rubber_Index = 0;

void Handle(void){
	Tab_main->bits.start = 0;
	Tab_main_indicator->bits.start =  1 ;
		Rubber_Index = 0;
		uint8_t item_count = 16;
		uint8_t count = 0;
		while(count < item_count){
			count++;
			wait_handler_stop();
			move_axis(Rubber[Rubber_Index].x, Rubber[Rubber_Index].y, max_z-4000);
			Input_Registers_Database[0] =  Rubber_Index+1 ;
			delay_us(500);
			wait_handler_stop();
			move_axis(Rubber[Rubber_Index].x, Rubber[Rubber_Index].y, max_z);
			delay_us(1000);

			Rubber_Index++;
		}
		wait_handler_stop();
		move_axis(0, 0, 0);
		Input_Registers_Database[0] =0 ;
		Input_Registers_Database[1] = 0;
}
#endif
