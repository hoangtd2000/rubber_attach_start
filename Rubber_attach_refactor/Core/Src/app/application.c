/*
 * application.c
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#include "application.h"

extern  uint8_t RxData[256];
extern UART_HandleTypeDef huart2;
extern volatile SystemFlag_t SystemFlag;
extern Axis_t AxisX, AxisY, AxisZ;
extern Tab_main_t* Tab_main;
extern Taskbar_t* Taskbar;
extern Tab_main_t* Tab_main_indicator;
extern Tab_popup_t* Tab_popup;
extern Popup_Indicator_t* Popup_Indicator ;
extern uint16_t* Mark;
Point2D Rubber_Tray[200] ;
Point2D Tray1[30];
Point2D Tray2[30];
const Point2D* Rubber = Rubber_Tray;
const Point2D* Tray_1 = Tray1;
const Point2D* Tray_2 = Tray2;
Point2D *TrayList[MAX_TRAYS] = { Tray1, Tray2 };

extern uint32_t data[10];
extern const uint32_t FlashStart;

extern Point2D Rubber_Mark[3];
extern Point2D Tray1_Mark[3];
extern Point2D Tray2_Mark[3];

PickState_t machine_state = ST_IDLE;
PickState_t prev_state = ST_IDLE;

uint16_t rubber_pair  = 0;   // đếm cặp trên khuôn cao su (0..99)
uint16_t tray_index   = 0;   // đếm số cặp đã bỏ vào tray (0..23)
uint8_t count_tray[MAX_TRAYS] = {0, 0};




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
	Calculate_TrayRubber_Point(Rubber_Tray, Rubber_Mark, RUBBER_ROWS, RUBBER_COLS);
	Calculate_Tray_Point(Tray1, Tray1_Mark, TRAY_ROWS, TRAY_COLS);
	Calculate_Tray_Point(Tray2, Tray2_Mark, TRAY_ROWS, TRAY_COLS);
}

void Calculate_TrayRubber_Point(Point2D* tray, const Point2D* point,uint8_t row, uint8_t col)
{
    if (row < 2 || col < 2) return;          // tránh chia 0
    const double dx_row = (double)(point[2].x - point[0].x) / (row - 1);
    const double dy_row = (double)(point[2].y - point[0].y) / (row - 1);
    const double dx_col = (double)(point[1].x - point[0].x) / (col - 1);
    const double dy_col = (double)(point[1].y - point[0].y) / (col - 1);

    uint16_t index = 0;

    for (uint8_t i = 0; i < row; ++i) {
        for (uint8_t j = 0; j < col; ++j) {
        	tray[index].y = point[0].y + i * dy_row + j * dy_col;
            tray[index].x = point[0].x + i * dx_row + j * dx_col;
            ++index;
        }
    }
}

void Calculate_Tray_Point(Point2D* tray, const Point2D* point,uint8_t row, uint8_t col)
{
    if (row < 2 || col < 2) return;          // tránh chia 0
    const double dx_row = (double)(point[2].x - point[0].x) / (row - 1);
    const double dy_row = (double)(point[2].y - point[0].y) / (row - 1);
    const double dx_col = (double)(point[1].x - point[0].x) / (col - 1);
    const double dy_col = (double)(point[1].y - point[0].y) / (col - 1);

    uint16_t index = 0;

    for (uint8_t i = 0; i < row; ++i) {
        for (uint8_t j = 0; j < col; ++j) {
        	if(i % 2 == 0){
        		tray[index].y = point[0].y + i * dy_row + j * dy_col;
        		tray[index].x = point[0].x + i * dx_row + j * dx_col;
        	}
        	else{
        		tray[index].y = point[0].y + i * dy_row + j * dy_col - Y_Calibrator;
        		tray[index].x = point[0].x + i * dx_row + j * dx_col - X_Calibrator;
        	}
            ++index;
        }
    }
}



void Handle(void)
{
	Tab_main->bits.start = 0;

	Tab_main_indicator->bits.start =  1 ;
	 Input_Registers_Database[3] = count_tray[0];
	 Input_Registers_Database[4] = count_tray[1];
//	Mark_rubber_working(count_tray[0]);
//	Mark_tray1_working(count_tray[1]);
	if(tray_index == 0){
		if(!Tab_main_indicator->bits.stop){
		Clear_all_tray1();
		Clear_all_tray2();
		}
	}
	if(rubber_pair == 0){
		if(!Tab_main_indicator->bits.stop){
		Mark_all_rubber();
		}
	}
	Tab_main_indicator->bits.stop =  0 ;
	while(tray_index < MAX_PAIRS && rubber_pair < RUBBER_TOTAL_PAIRS) // Dừng khi đầy tray1,2 và hết hàng ở tray rubber
    {
		Tab_main->bits.start = 0;
		if(SystemFlag.is_stop && machine_state != ST_PAUSE){
			prev_state = machine_state;
			machine_state = ST_PAUSE;
		}
		switch(machine_state)
		{
			case ST_IDLE:
			{
				Tab_main_indicator->bits.start =  1 ;
				Close_Popup(popup_err);
				machine_state = ST_MOVE_TO_RUBBER;
				break;
			}
			case ST_MOVE_TO_RUBBER:
			{
				OFF_LED_RED;
				ON_LED_GREEN;
				Tab_main_indicator->bits.start =  1 ;
				if(tray_index >= MAX_PAIRS || rubber_pair >= RUBBER_TOTAL_PAIRS)
				{
					machine_state = ST_STOP;
					break;
				}

				int pair_row = rubber_pair / RUBBER_COLS;
				int pair_col = rubber_pair % RUBBER_COLS;

				int rx = (pair_row & 1) ? (RUBBER_COLS - 1 - pair_col) : pair_col;
				int ry = pair_row * 2;

				// ===== Pick Rubber =====
				wait_handler_stop();
				move_axis(Rubber[ry * RUBBER_COLS + rx].x, Rubber[ry * RUBBER_COLS + rx].y, max_z_rubber - z_up);
				//delay_us(1000);

				wait_handler_stop();

				move_axis1(Rubber[ry * RUBBER_COLS + rx].x, Rubber[ry * RUBBER_COLS + rx].y, max_z_rubber);
				wait_handler_stop();
			//	delay_us(1000);
				machine_state = ST_PICK1;
				break;
			}
			case ST_PICK1:
			{
				delay_us(500);
				SetPickRubber(0);
				SetPickRubber(1);
			    machine_state = ST_WAIT_PICK1;
			    break;
			}
			case ST_WAIT_PICK1:
			{
				int pair_row = rubber_pair / RUBBER_COLS;
				int pair_col = rubber_pair % RUBBER_COLS;

				int rx = (pair_row & 1) ? (RUBBER_COLS - 1 - pair_col) : pair_col;
				int ry = pair_row * 2;
			    if (Handle_Pick[0].result == OK)
			    {
			        Clear_mark_rubber(ry * RUBBER_COLS + rx);
			        machine_state = ST_PICK2;
			    }
			    else if (Handle_Pick[0].result == NG){
			    	Mark_rubber(ry * RUBBER_COLS + rx);
			    	SetReleaseRubber(0);
			    	SetReleaseRubber(1);
					Open_Popup(0);
					SetBips(3);
					machine_state = ST_WAIT_POPUP;
			    }
				break;
			}
			case ST_PICK2:
			{
				//SetPickRubber(1);
				machine_state = ST_WAIT_PICK2;
			    break;
			}
			case ST_WAIT_PICK2:
			{
				int pair_row = rubber_pair / RUBBER_COLS;
				int pair_col = rubber_pair % RUBBER_COLS;

				int rx = (pair_row & 1) ? (RUBBER_COLS - 1 - pair_col) : pair_col;
				int ry = pair_row * 2;
			    if (Handle_Pick[1].result == OK)
			    {
				    Clear_mark_rubber(ry * RUBBER_COLS + rx + RUBBER_COLS );
				    machine_state = ST_PLACE1;
			    }
			    else if(Handle_Pick[1].result == NG){
			    	SystemFlag.is_err = 1 ;
			    	Mark_rubber(ry * RUBBER_COLS + rx);
			    	Mark_rubber(ry * RUBBER_COLS + rx + RUBBER_COLS );
			    	SetReleaseRubber(0);
			    	SetReleaseRubber(1);
			        Open_Popup(popup_err);
			        SetBips(3);
			        machine_state = ST_WAIT_POPUP;
			    }
				break;
			}
			case ST_WAIT_POPUP:
			{
//				if(Timer_Check(1, 500) && Inputs_Database[34]){
				if(Timer_Check(1, 500) && Popup_Indicator->bits.err){
					OFF_LED_GREEN;
					TOGGLE_LED_RED;
				}
				if(Tab_popup->bits.stop ==  1)
					{
						SystemFlag.is_err = 0 ;
						Tab_popup->bits.stop = 0;
						Tab_main_indicator->bits.stop =  1 ;
						Close_Popup(popup_err);
						rubber_pair++;   // bỏ cả cặp lỗi
						machine_state = ST_STOP;
					}
					if(Tab_popup->bits.next ==  1)
					{
						SystemFlag.is_err = 0 ;
						Tab_popup->bits.next = 0;
						Close_Popup(popup_err);
						rubber_pair++;   // bỏ cả cặp lỗi
						machine_state = ST_MOVE_TO_RUBBER;
					}
				break;
			}
			case ST_PLACE1:
			{
			    uint8_t tray_id   = tray_index / PAIRS_PER_TRAY;
			    uint8_t tray_pair = tray_index % PAIRS_PER_TRAY;

			    int tx = tray_pair % TRAY_COLS;
			    int ty = (tray_pair / TRAY_COLS) * 2;

			    Point2D *tray = TrayList[tray_id];

			    PlaceToTray(tray, tray_id, ty * TRAY_COLS + tx);
			    machine_state = ST_RELEASE1;
			    break;
			}
			case ST_RELEASE1:
			{
				wait_handler_stop();
				delay_us(200);
				SetReleaseRubber(0);
				machine_state = ST_WAIT_RELEASE1;
			    break;
			}
			case ST_WAIT_RELEASE1:
			{
				if(Handle_Release[0].result == OK){
					machine_state = ST_PLACE2;
				}
			    else if (Handle_Release[0].result == NG)
			    {
			        // OpenPopup
			    }
			    break;
			}
			case ST_PLACE2:
			{
			    uint8_t tray_id   = tray_index / PAIRS_PER_TRAY;
			    uint8_t tray_pair = tray_index % PAIRS_PER_TRAY;

			    int tx = tray_pair % TRAY_COLS;
			    int ty = (tray_pair / TRAY_COLS) * 2;

			    Point2D *tray = TrayList[tray_id];

			    PlaceToTray(tray, tray_id, (ty + 1) * TRAY_COLS + tx);
			    machine_state = ST_RELEASE2;
			    break;
			}
			case ST_RELEASE2:
			{
				wait_handler_stop();
				delay_us(200);
				SetReleaseRubber(1);
			    machine_state = ST_WAIT_RELEASE2;
			    break;
			}
			case ST_WAIT_RELEASE2:
			{
			    if (Handle_Release[1].result == OK)
			    {
			        machine_state = ST_NEXT_PAIR;
			    }
			    else if (Handle_Release[1].result == NG)
			    {
			    	// OpenPopup();
			    }
			    break;
			}
			case ST_NEXT_PAIR:
			{
				rubber_pair++;
				tray_index++;
				delay_us(200);
				machine_state = ST_MOVE_TO_RUBBER;
				break;
			}
			case ST_STOP:
			{
				wait_handler_stop();
				move_axis(0, 0, 0);
				wait_handler_stop();
				Tab_main_indicator->bits.start = 0;
				if(Timer_Check(1, 500)){
					OFF_LED_GREEN;
					TOGGLE_LED_RED;
				}
				if(Tab_main->bits.start == 1){
					Tab_main->bits.start = 0;
				Tab_main_indicator->bits.stop =  0 ;
					machine_state = ST_MOVE_TO_RUBBER;
				}
				break;
			}
			case ST_PAUSE:
			{
				wait_handler_stop();
				  Vacum1_Pick_Off;
				  Vacum1_Release_Off;
				  Vacum2_Pick_Off;
				  Vacum2_Release_Off;
				Tab_main_indicator->bits.start = 0;
			    if(Timer_Check(1, 500))
			    {
			        OFF_LED_GREEN;
			        TOGGLE_LED_RED;
			        TOGGLE_BUZZ;
			    }
			    SystemFlag.is_stop = 0;
			    if (!SystemFlag.is_stop)
			    {
			        OFF_BUZZ;
			        if (prev_state == ST_WAIT_POPUP)
			        {
			            machine_state = ST_WAIT_POPUP;
			        }
			        else if (Tab_main->bits.start == 1)
			        {
			            ON_LED_GREEN;
			            OFF_LED_RED;
			            Tab_main_indicator->bits.stop = 0 ;
			            Tab_main_indicator->bits.start = 1;
			            machine_state = prev_state;
			        }
			    }
			    break;
			}
			case ST_PAUSE_DOOR:
			{
				wait_handler_stop();
				Tab_main_indicator->bits.start = 0;
			    if(Timer_Check(1, 500))
			    {
			        OFF_LED_GREEN;
			        TOGGLE_LED_RED;
			        TOGGLE_BUZZ;
			    }
			    if (!DOOR_OPEN())
			    {
			        OFF_BUZZ;
			        if (prev_state == ST_WAIT_POPUP)
			        {
			            machine_state = ST_WAIT_POPUP;
			        }
			        else if (Tab_main->bits.start == 1)
			        {
			            ON_LED_GREEN;
			            OFF_LED_RED;
			            Tab_main_indicator->bits.start = 1;
			            machine_state = prev_state;
			        }
			    }
			    break;
			}
			default:
				break;
		}
    }
	SystemFlag.is_stop = 0 ;

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

void PlaceToTray(Point2D *tray, uint8_t tray_id, int index)
{
    wait_handler_stop();
    move_axis(tray[index].x, tray[index].y, max_z_tray - z_up);

//    delay_us(500);
    wait_handler_stop();
    count_tray[tray_id]++;
    if(tray_id == 0){
        Mark_tray1(index);
        Input_Registers_Database[3] = count_tray[0];
  //      Mark_tray1_working(count_tray[0]);

    } else {
        Mark_tray2(index);
    //    Mark_tray2_working(count_tray[1]) ;
        Input_Registers_Database[4] = count_tray[1];
    }
    move_axis1(tray[index].x, tray[index].y, max_z_tray);
    wait_handler_stop();
  //  delay_us(1000);
}

void application_init(){
//	Cylinder1_Go_Up;
//	Cylinder2_Go_Up;
//
//	  Vacum1_Pick_Off;
//	  Vacum1_Release_Off;
//	  Vacum2_Pick_Off;
//	  Vacum2_Release_Off;

		HAL_Delay(7000);
		Mark_all_rubber();
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxData, 256);
		Taskbar->bits.home = 1 ;
		Read_Tray_Data();
		HAL_TIM_Base_Start_IT(&htim5); //x
		HAL_TIM_Base_Start_IT(&htim9); //y
		HAL_TIM_Base_Start_IT(&htim2); //z
		HAL_TIM_Base_Start_IT(&htim6); // kiem tra hmi
		HAL_TIM_Base_Start_IT(&htim7); // kiem tra trang thai x, y, z
		Set_Speed_Motor_x( speed_default, speed_x_max);
		Set_Speed_Motor_y( speed_default, speed_y_max);
		Set_Speed_Motor_z( speed_default, speed_z_max);
		reset_counter_timer_x();
		reset_counter_timer_slave_x();
		reset_counter_timer_y();
		reset_counter_timer_slave_y();
		reset_counter_timer_z();
		reset_counter_timer_slave_z();
		Try_go_home();

}
void Try_go_home(){
		SystemFlag.is_homing = 1 ;
		Open_Popup(popup_home);

	  if(get_home_z() == home_z){
		  AxisZ.mode = MOVE_HOME2;
	  }else {
		  AxisZ.mode = MOVE_HOME1;
	  }
		while((AxisZ.mode != MOVE_HOME3));

	  if(get_home_x() == home_x){
		  AxisX.mode = MOVE_HOME2;
	  }else{
		  AxisX.mode = MOVE_HOME1;
	  }
	  if(get_home_y() == home_y){
		  AxisY.mode = MOVE_HOME2;
	  }else{
		  AxisY.mode = MOVE_HOME1;
	  }
	  wait_handler_stop();
	  Close_Popup(popup_home);
	  SystemFlag.is_homing = 0 ;
	  SystemFlag.is_err = 0 ;
	  SystemFlag.is_start = 0 ;
	  SystemFlag.is_stop = 0;
}

void application_run_main(void){
	  if(Timer_Check(0, 500)){
		  OFF_LED_RED;
	//	  OFF_BUZZ;
		  TOGGLE_LED_GREEN;
	  }
	  else if(Timer_Check(2, 500)  && SystemFlag.is_err){
		  OFF_LED_GREEN;
		  TOGGLE_LED_RED;
		//  TOGGLE_BUZZ;
	  }
}


void task_timer6(){
	if(Taskbar->bits.home){
		Handle_main();
	}else if(Taskbar->bits.motor){
		Handle_motor();
	}
}
void task_timer7(){
	Control_motor_y();
	Control_motor_x();
	Control_motor_z();
	BipControl();
	PickRubber1(0);
	PickRubber1(1);
	ReleaseRubber1(0);
	ReleaseRubber1(1);
	  if(Tab_main->bits.stop){
		  Handle_stop();
	  }
}







