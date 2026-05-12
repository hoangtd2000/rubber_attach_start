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

extern uint16_t data[FLASH_MODEL_WORDS];
extern const uint32_t FlashStart;

extern Point3D Rubber_Mark[3];
extern Point3D Tray1_Mark[3];
extern Point3D Tray2_Mark[3];

PickState_t machine_state = ST_IDLE;
PickState_t prev_state = ST_IDLE;

//GripOffset_t GripOffset[ITEMS_PER_PICK] = {
//    {  0,   	0 },    // 1
//    { -4000,   	0 },    // 2
//    { -4000,-3500 },  // 3
//    { 0,  	-3500 },   // 4
//    { 0,    -7000 },   // 5
//    { -4000,-7000 }   // 6
//};

GripOffset_t GripOffset[ITEMS_PER_PICK] = {
    {  0,   	0 },    // 1
    { -3980,   	0 },    // 2   >4000 trai
    { -3990,-3490 },  // 3
    { 0,  	-3490 },   // 4
    { -30,    -7000 },   // 5
    { -3950,-7000 }   // 6
};

volatile uint16_t tray_index     = 0;   // 0..199
volatile uint8_t  grip_id = 0;   // 0..5
volatile uint8_t  slot       = 0;   // 0..7
volatile uint8_t count_tray[MAX_TRAYS] = {0, 0};

uint32_t t_start = 0;
uint32_t t_end = 0;
double t_cycletime = 0;
uint32_t total = 0;


ModelConfig_t ModelConfigs[MAX_MODELS] = {
    {0, 10, 20},  	// Model 0: 10 rows, 20 cols
    {1, 10, 20},   	// Model 1: 10 rows, 20 cols
    {2, 10, 20},  	// Model 2: 10 rows, 20 cols
    {3, 6, 12}    	// Model 3: 6 rows, 12 cols
};

uint8_t GetCurrentModelRows(void) {
    uint16_t model = data[0];
    if (model >= MAX_MODELS) model = 0;
    return ModelConfigs[model].rows;
}

uint8_t GetCurrentModelCols(void) {
    uint16_t model = data[0];
    if (model >= MAX_MODELS) model = 0;
    return ModelConfigs[model].cols;
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

void LoadMarkFromFlash(Point3D *mark,
                       uint32_t *flashData,
                       uint8_t offset,
                       uint8_t count)
{
    for(uint8_t i = 0; i < count; i++)

    {
        mark[i].raw = flashData[i + offset];
    }
}

void Read_Tray_Data(void)
{
    Flash_Read_Data(FlashStart, data, (uint16_t)FLASH_MODEL_WORDS);

    uint16_t model = data[0];
    if (model >= MAX_MODELS) {
        model = 0;
        data[0] = 0;
    }
    Holding_Registers_Database[39] = model;
    uint16_t blockStart = 1 + model * MODEL_DATA_WORDS;
    uint16_t rowCol = data[blockStart];
    uint8_t rows = (uint8_t)(rowCol >> 8);
    uint8_t cols = (uint8_t)(rowCol & 0xFF);

    if (rows < 2 || cols < 2 || rows > 10 || cols > 20) {
        rows = ModelConfigs[model].rows;
        cols = ModelConfigs[model].cols;
        data[blockStart] = ((uint16_t)rows << 8) | cols;
    }

    for (uint8_t i = 0; i < 3; i++) {
        uint16_t base = blockStart + 1 + i * 3;
        Rubber_Mark[i].x = data[base + 0];
        Rubber_Mark[i].y = data[base + 1];
        Rubber_Mark[i].z = data[base + 2];
    }

    for (uint8_t i = 0; i < 3; i++) {
        uint16_t base = blockStart + 10 + i * 3;
        Tray1_Mark[i].x = data[base + 0];
        Tray1_Mark[i].y = data[base + 1];
        Tray1_Mark[i].z = data[base + 2];
    }

    for (uint8_t i = 0; i < 3; i++) {
        uint16_t base = blockStart + 19 + i * 3;
        Tray2_Mark[i].x = data[base + 0];
        Tray2_Mark[i].y = data[base + 1];
        Tray2_Mark[i].z = data[base + 2];
    }

    CopyMarkToArray(&Mark[0], Rubber_Mark, 3);
    CopyMarkToArray(&Mark[9], Tray1_Mark, 3);
    CopyMarkToArray(&Mark[18], Tray2_Mark, 3);

    Calculate_TrayRubber_Point(Rubber_Tray, Rubber_Mark, rows, cols);
//    Calculate_Tray_Point(Tray1, Tray1_Mark, rows, cols);
//    Calculate_Tray_Point(Tray2, Tray2_Mark, rows, cols);
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
	t_start = Timer_get();
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

//				int rx = (row & 1) ? (RUBBER_COLS - 1 - col) : col;
//				int ry = row;

				int rx = col;
				int ry = row;

			    MoveToRubber(ry * RUBBER_COLS + rx, grip_id);
			    //t_start = Timer_get();
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
			    if (Handle_Pick[grip_id].result == OK)
			    {
			    	Clear_mark_rubber(tray_index);
			    	grip_id++;
			    	tray_index++;
			    	machine_state = ST_CHECK_GRIP;
			    }
			    else if(Handle_Pick[grip_id].result == NG)
			    {
			    	SetReleaseRubber(grip_id);
			    	delay_us(1000);
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
				//delay_us(500);
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
				delay_us(300);
				SetReleaseRubber(0);
				SetReleaseRubber(1);
				SetReleaseRubber(2);
				SetReleaseRubber(3);
				SetReleaseRubber(4);
				SetReleaseRubber(5);
				machine_state = ST_CHECK_RELEASE_ALL;
				break;
			}
			case ST_CHECK_RELEASE_ALL:
			{
			    if(Handle_Release[0].result == OK &&
			       Handle_Release[1].result == OK &&
			       Handle_Release[2].result == OK &&
			       Handle_Release[3].result == OK &&
			       Handle_Release[4].result == OK &&
			       Handle_Release[5].result == OK)
			    {
//			    	t_end = Timer_get();
//			    	t_cycletime = (double)(t_end - t_start) / 10.0;
//			    	Holding_Registers_Database[41] = t_cycletime;
			    	Holding_Registers_Database[42] += 6;
			        grip_id = 0;
			        machine_state = ST_NEXT_SLOT;
			    }
			    break;
			}

			case ST_NEXT_SLOT:
			{
				slot++;
				delay_us(200);
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
	t_end = Timer_get();
	t_cycletime = (double)(t_end - t_start) / 10.0;
	Holding_Registers_Database[41] = t_cycletime / 48;
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
    //move_axis(tray[index].x, tray[index].y, max_z_tray - z_up);
    move_axis(tray[index].x, tray[index].y, tray[index].z);
    wait_handler_stop();
    count_tray[tray_id]++;
    if(tray_id == 0){
        Mark_tray1(index);
        Mark_tray1(index+1);
        Mark_tray1(index+4);
        Mark_tray1(index+5);
        Mark_tray1(index+8);
        Mark_tray1(index+9);
        //Input_Registers_Database[3] = count_tray[0];
    } else {
        Mark_tray2(index);
        Mark_tray2(index+1);
        Mark_tray2(index+4);
        Mark_tray2(index+5);
        Mark_tray2(index+8);
        Mark_tray2(index+9);
        //Input_Registers_Database[4] = count_tray[1];
    }
    //move_axis1(tray[index].x, tray[index].y, max_z_tray);
    move_axis1(tray[index].x, tray[index].y, tray[index].z);
    wait_handler_stop();
}

void MoveToRubber(int rubber_index, int grip_id)
{
    wait_handler_stop();

    int x = Rubber_Tray[rubber_index].x + GripOffset[grip_id].dx;
    int y = Rubber_Tray[rubber_index].y + GripOffset[grip_id].dy;
    int z = Rubber_Tray[rubber_index].z + GripOffset[grip_id].dz;

    //move_axis(x, y, max_z_rubber - z_up);
    move_axis(x, y, z);
    wait_handler_stop();

    //move_axis1(x, y, max_z_rubber);
    move_axis1(x, y, z);
    wait_handler_stop();
}

void application_init(){

	HAL_Delay(7000);
	Mark_all_rubber();

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxData, 256);
	Taskbar->bits.main = 1 ;
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
	//		  SystemFlag.is_homing = 0 ;
	//		  SystemFlag.is_err = 0 ;
	//		  SystemFlag.is_start = 0 ;
	//		  SystemFlag.is_stop = 0;

}
void Try_go_home(){
		SystemFlag.is_homing = 1 ;
		Open_Popup(popup_home);

	  if(get_home_z() == home_z){
		  AxisZ.mode = MOVE_HOME2;
	  }else {
		  AxisZ.mode = MOVE_HOME1;
	  }
	  Home_process_z();
		while((AxisZ.mode != MOVE_HOME3));
		while((AxisZ.mode != STOP));
	  if(get_home_x() == home_x){
		  AxisX.mode = MOVE_HOME2;
	  }else{
		  AxisX.mode = MOVE_HOME1;
	  }
	  Home_process_x();
	  if(get_home_y() == home_y){
		  AxisY.mode = MOVE_HOME2;
	  }else{
		  AxisY.mode = MOVE_HOME1;
	  }
	  Home_process_y();
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
		  TOGGLE_LED_GREEN;
	  }
	  else if(Timer_Check(2, 500)  && SystemFlag.is_err){
		  OFF_LED_GREEN;
		  TOGGLE_LED_RED;
	  }
		if(Taskbar->bits.main){
			Handle_main();
		}else if(Taskbar->bits.motor){
			Handle_motor();
		}else if(Taskbar->bits.setting){
			Handle_setting();
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







