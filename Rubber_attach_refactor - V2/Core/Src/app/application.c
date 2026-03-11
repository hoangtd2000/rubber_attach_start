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
Point3D Rubber_Tray[200] ;
Point3D Tray1[30];
Point3D Tray2[30];
const Point3D* Rubber = Rubber_Tray;
const Point3D* Tray_1 = Tray1;
const Point3D* Tray_2 = Tray2;
Point3D *TrayList[MAX_TRAYS] = { Tray1, Tray2 };

extern uint32_t data[10];
extern const uint32_t FlashStart;

extern Point3D Rubber_Mark[3];
extern Point3D Tray1_Mark[3];
extern Point3D Tray2_Mark[3];

PickState_t machine_state = ST_IDLE;
PickState_t prev_state = ST_IDLE;

GripOffset_t GripOffset[ITEMS_PER_PICK] = {
    {  0,   	0 },    // 1
    { -4000,   	0 },    // 2
    { -4000,-3500 },  // 3
    { 0,  	-3500 },   // 4
    { 0,    -7000 },   // 5
    { -4000,-7000 }   // 6
};

volatile uint16_t tray_index     = 0;   // 0..199
volatile uint8_t  grip_id = 0;   // 0..5
volatile uint8_t  slot       = 0;   // 0..7
volatile uint8_t count_tray[MAX_TRAYS] = {0, 0};

void Read_Index_Rubber(){
	tray_index = Holding_Registers_Database[3];
	Set_item_rubber(tray_index);
	Clear_item_rubber(tray_index);
}
void Read_Index_Tray(){
	slot = Holding_Registers_Database[4];
	Clear_item_tray(slot);
	Set_item_tray(slot);
}
void Set_Possition(){
	//if(){ // kểm tra bit được set trên màn hình
		Read_Index_Rubber();
		Read_Index_Tray();
	//}
}

static void CopyMarkToArray(uint16_t *dst, Point3D *src, uint8_t count)
{
    for(uint8_t i = 0; i < count; i++)
    {
        dst[i*3 + 0] = src[i].x;
        dst[i*3 + 1] = src[i].y;
        dst[i*3 + 2] = src[i].z;
    }
}
void Read_Tray_Data(){
	Flash_Read_Data( FlashStart, data, 10);
	// Gán raw
	for(uint8_t i = 0; i < 3; i++)
	{
		Rubber_Mark[i].raw = data[i];
		Tray1_Mark[i].raw  = data[i + 3];
		Tray2_Mark[i].raw  = data[i + 6];
	}
	// Copy ra mảng Mark
	CopyMarkToArray(&Mark[0],  Rubber_Mark, 3);
	CopyMarkToArray(&Mark[9],  Tray1_Mark,  3);
	CopyMarkToArray(&Mark[18], Tray2_Mark,  3);
	Calculate_TrayRubber_Point(Rubber_Tray, Rubber_Mark, RUBBER_ROWS, RUBBER_COLS);
	Calculate_Tray_Point(Tray1, Tray1_Mark, TRAY_ROWS, TRAY_COLS);
	Calculate_Tray_Point(Tray2, Tray2_Mark, TRAY_ROWS, TRAY_COLS);
}

void Calculate_TrayRubber_Point(Point3D* tray,
                                const Point3D* point,
                                uint8_t row,
                                uint8_t col)
{
    if (row < 2 || col < 2) return;

    const float dx_row = ((float)point[2].x - point[0].x) / (row - 1);
    const float dy_row = ((float)point[2].y - point[0].y) / (row - 1);
    const float dz_row = ((float)point[2].z - point[0].z) / (row - 1);

    const float dx_col = ((float)point[1].x - point[0].x) / (col - 1);
    const float dy_col = ((float)point[1].y - point[0].y) / (col - 1);
    const float dz_col = ((float)point[1].z - point[0].z) / (col - 1);

    uint16_t index = 0;

    for (uint8_t i = 0; i < row; ++i)
    {
        for (uint8_t j = 0; j < col; ++j)
        {
            float x = point[0].x + i * dx_row + j * dx_col;
            float y = point[0].y + i * dy_row + j * dy_col;
            float z = point[0].z + i * dz_row + j * dz_col;

            tray[index].x = (uint16_t)x;
            tray[index].y = (uint16_t)y;
            tray[index].z = (uint16_t)z;

            index++;
        }
    }
}

void Calculate_Tray_Point(Point3D* tray,
                          const Point3D* point,
                          uint8_t row,
                          uint8_t col)
{
    if (row < 2 || col < 2) return;

    const float dx_row = ((float)point[2].x - point[0].x) / (row - 1);
    const float dy_row = ((float)point[2].y - point[0].y) / (row - 1);
    const float dz_row = ((float)point[2].z - point[0].z) / (row - 1);

    const float dx_col = ((float)point[1].x - point[0].x) / (col - 1);
    const float dy_col = ((float)point[1].y - point[0].y) / (col - 1);
    const float dz_col = ((float)point[1].z - point[0].z) / (col - 1);

    uint16_t index = 0;

    for (uint8_t i = 0; i < row; ++i)
    {
        for (uint8_t j = 0; j < col; ++j)
        {
            float x = point[0].x + i * dx_row + j * dx_col;
            float y = point[0].y + i * dy_row + j * dy_col;
            float z = point[0].z + i * dz_row + j * dz_col;

//            if (i % 2 != 0)
//            {
//                x -= X_Calibrator;
//                y -= Y_Calibrator;
//                z -= Z_Calibrator;
//            }

            tray[index].x = (uint16_t)x;
            tray[index].y = (uint16_t)y;
            tray[index].z = (uint16_t)z;

            index++;
        }
    }
}

void Handle(void)
{
	Tab_main->bits.start = 0;

	Tab_main_indicator->bits.start =  1 ;
	Input_Registers_Database[3] = count_tray[0];
	Input_Registers_Database[4] = count_tray[1];
	if(slot == 0){
		if(!Tab_main_indicator->bits.stop){
		Clear_all_tray1();
		Clear_all_tray2();
		}
	}
	if(tray_index == 0){
		if(!Tab_main_indicator->bits.stop){
		Mark_all_rubber();
		}
	}
	Tab_main_indicator->bits.stop =  0 ;
	while(tray_index < TOTAL_SLOTS_RUBBER  && slot < TOTAL_SLOTS)
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
				machine_state = ST_PICK_MOVE;
				break;
			}
			case ST_PICK_MOVE:
			{
				Tab_main_indicator->bits.start =  1 ;
			    if (tray_index >= TOTAL_SLOTS_RUBBER || slot >= TOTAL_SLOTS)
			    {
			        machine_state = ST_STOP;
			        break;
			    }

				int row = tray_index / RUBBER_COLS;
				int col = tray_index % RUBBER_COLS;

				int rx = (row & 1) ? (RUBBER_COLS - 1 - col) : col;
				int ry = row;

			    MoveToRubber(ry * RUBBER_COLS + rx, grip_id);
				machine_state = ST_PICK;
				break;
			}
			case ST_PICK:
			{
				delay_us(300);
			    SetPickRubber(grip_id);
			    machine_state = ST_WAIT_PICK;
				break;
			}
			case ST_WAIT_PICK:
			{
				//delay_us(500);
			    if (Handle_Pick[grip_id].result == OK)
			    {
			    	Clear_mark_rubber(tray_index);
			    	grip_id++;
			    	tray_index++;
			    	machine_state = ST_CHECK_GRIP;
			    }
			    else if(Handle_Pick[grip_id].result == NG)
			    {
			    	Mark_rubber(tray_index);
			    	tray_index++;
			    	machine_state = ST_PICK_MOVE;
			    }
			    break;
			}
			case ST_CHECK_GRIP:
			{
				if(grip_id >= ITEMS_PER_PICK)
				{
					machine_state = ST_CHECK_FULL;
				}
				else
				{
					machine_state = ST_PICK_MOVE;
				}
				break;
			}
			case ST_CHECK_FULL:
			{
				delay_us(500);
				machine_state = ST_MOVE_TO_TRAY;
				break;
			}
			case ST_MOVE_TO_TRAY:
			{
			    uint8_t tray_id   = slot / SLOTS_PER_TRAY; // 0 or 1
			    uint8_t slot_id   = slot % SLOTS_PER_TRAY; // 0..3

			    int block_row = slot_id / (TRAY_COLS / GRIP_COL);
			    int block_col = slot_id % (TRAY_COLS / GRIP_COL);

			    int tx = block_col * GRIP_COL;
			    int ty = block_row * GRIP_ROW;

			    Point3D *tray = TrayList[tray_id];

			    MoveToTray(tray, tray_id, ty * TRAY_COLS + tx);
			    machine_state = ST_RELEASE_ALL;
			}

			case ST_RELEASE_ALL:
			{
			    if(Handle_Release[0].result == OK &&
			       Handle_Release[1].result == OK &&
			       Handle_Release[2].result == OK &&
			       Handle_Release[3].result == OK &&
			       Handle_Release[4].result == OK &&
			       Handle_Release[5].result == OK)
			    {
			        grip_id = 0;
			        machine_state = ST_NEXT_SLOT;
			    }
			    break;
			}

			case ST_NEXT_SLOT:
			{
				slot++;
				delay_us(500);
				machine_state = ST_PICK_MOVE;
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
					machine_state = ST_PICK_MOVE;
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
			        if (Tab_main->bits.start == 1)
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
	if(slot >= TOTAL_SLOTS){
		slot  = 0;
	}
	if(tray_index >= TOTAL_SLOTS_RUBBER){
		tray_index  = 0;
		count_tray[0] = 0;
		count_tray[1] = 0;
	}
}

void MoveToTray(Point3D *tray, uint8_t tray_id, int index)
{
    wait_handler_stop();
    move_axis(tray[index].x, tray[index].y, max_z_tray - z_up);

    wait_handler_stop();
    count_tray[tray_id]++;
    if(tray_id == 0){
        Mark_tray1(index);
        Mark_tray1(index+1);
        Mark_tray1(index+4);
        Mark_tray1(index+5);
        Mark_tray1(index+8);
        Mark_tray1(index+9);
        Input_Registers_Database[3] = count_tray[0];
    } else {
        Mark_tray2(index);
        Mark_tray2(index+1);
        Mark_tray2(index+4);
        Mark_tray2(index+5);
        Mark_tray2(index+8);
        Mark_tray2(index+9);
        Input_Registers_Database[4] = count_tray[1];
    }
    move_axis1(tray[index].x, tray[index].y, max_z_tray);
    wait_handler_stop();
}

void MoveToRubber(int rubber_index, int grip_id)
{
    wait_handler_stop();

    int x = Rubber_Tray[rubber_index].x + GripOffset[grip_id].dx;
    int y = Rubber_Tray[rubber_index].y + GripOffset[grip_id].dy;

    move_axis(x, y, max_z_rubber - z_up);
    wait_handler_stop();

    move_axis1(x, y, max_z_rubber);
    wait_handler_stop();
}

void application_init(){
	HAL_Delay(6000);
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
	if(Taskbar->bits.home){
		Handle_main();
	}else if(Taskbar->bits.motor){
		Handle_motor();
	}
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

}
void task_timer7(){
	Control_motor_y();
	Control_motor_x();
	Control_motor_z();
	BipControl();
	PickRubber1(0);
	PickRubber1(1);
	PickRubber1(2);
	PickRubber1(3);
	PickRubber1(4);
	PickRubber1(5);
	ReleaseRubber1(0);
	ReleaseRubber1(1);
	ReleaseRubber1(2);
	ReleaseRubber1(3);
	ReleaseRubber1(4);
	ReleaseRubber1(5);
	  if(Tab_main->bits.stop){
		  Handle_stop();
	  }
}







