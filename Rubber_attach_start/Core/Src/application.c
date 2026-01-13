/*
 * application.c
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#include "application.h"

extern uint8_t RxData[256];
extern uint8_t TxData[256];

extern ScreenMain_t* ScreenMain;
extern Control_motor_t* Control_motor;
extern Cylinder_and_save_t* Cylinder_and_save;
extern Rubber_and_tray_t* Rubber_and_tray;


extern Axis_t AxisX, AxisY, AxisZ;
extern uint16_t Input_Registers_Database[50];
extern uint8_t Inputs_Database[25];
extern uint16_t* Mark;
extern const uint32_t FlashStart;


Taskbar_t* Taskbar = (Taskbar_t*)&Coils_Database[0];
Tab_main_t* Tab_main = (Tab_main_t*)&Coils_Database[5];

Tab_main_t* Tab_main_indicator = (Tab_main_t*) &Inputs_Database[0];

ActionHandler_t Tab_main_table[] =  {
		 Handle_reset,
		 Handle_start,
		 Handle_stop,
};

ActionHandler_t Tab_motor_table[] =  {
		 Handle_X_Left,
		 Handle_X_Right,
		 Handle_Y_Backward,
		 Handle_Y_Forward,
		 Handle_Z_Up,
		 Handle_Z_Down,
		 Handle_Set,
		 Handle_Home,
		 Handle_pick_handler1,
		 Handle_release_handler1,
		 Handle_pick_handler2,
		 Handle_release_handler2,
		 Handle_save1,
		 Handle_save2,
		 Handle_save3,
		 Handle_load,
		 Handle_tray_rubber_p1,
		 Handle_tray_rubber_p2,
		 Handle_tray_rubber_p3,
		 Handle_tray1_p1,
		 Handle_tray1_p2,
		 Handle_tray1_p3,
		 Handle_tray2_p1,
		 Handle_tray2_p2,
		 Handle_tray2_p3,
};

void application_init(){
		HAL_Delay(3000);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxData, 256);
		Read_Tray_Data();
		HAL_TIM_Base_Start_IT(&htim5); //x
		HAL_TIM_Base_Start_IT(&htim9); //y
		HAL_TIM_Base_Start_IT(&htim2); //z
		HAL_TIM_Base_Start_IT(&htim6);
		HAL_TIM_Base_Start_IT(&htim7);
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
	  if(get_home_z() == home_z){
		  AxisZ.mode = MOVE_HOME2;
	  }else {
		  AxisZ.mode = MOVE_HOME1;
	  }
}


void Handle_main(void){



	uint8_t builtin_Handle_main = __builtin_ffs(Tab_main->all);
		if (builtin_Handle_main > 0) {
			builtin_Handle_main -= 1;
		    if (builtin_Handle_main < (int)(sizeof(Tab_main_table) / sizeof(Tab_main_table[0]))) {
		    	Tab_main_table[builtin_Handle_main]();
		    }
		}
}
void Handle_motor(void){
	uint8_t builtin_Handle_motor = __builtin_ffs(Rubber_and_tray->all << 16 | Cylinder_and_save->all<< 8 | Control_motor->all);
		if (builtin_Handle_motor > 0) {
			builtin_Handle_motor -= 1;
		    if (builtin_Handle_motor < (int)(sizeof(Tab_motor_table) / sizeof(Tab_motor_table[0]))) {
		    	Tab_motor_table[builtin_Handle_motor]();
		    }
		}
}
void Handle_setting(void){

}


void Handle_reset(void){
	//Tab_main_indicator->bits.reset =  1 ;
}

void Handle_start(void){
	Handle();

	//Tab_main_indicator->bits.start =  1 ;
}
void Handle_stop(void){
	//Tab_main_indicator->bits.stop =  1 ;
}


void task_timer6(){
	if(Taskbar->bits.home){
		Handle_main();
	}else if(Taskbar->bits.motor){
		Handle_motor();
	}
//	else if(Taskbar->bits.SETTING == 1){
//
//	}
}
void task_timer7(){
	Control_motor_z();
	Control_motor_y();
	Control_motor_x();
}


void application_run_main(void){
//	for(int i = 1 ; i < 201 ; i++){
//	Input_Registers_Database[0] = i;
//	HAL_Delay(100);
//	}
//	for(int j = 0 ; j < 25 ; j++){
//	Input_Registers_Database[1] = j;
//	HAL_Delay(300);
//	}
//	for(int z = 0 ; z < 25 ; z++){
//	Input_Registers_Database[2] = z;
//	HAL_Delay(300);
//	}

}

