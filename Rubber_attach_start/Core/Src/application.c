/*
 * application.c
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#include "application.h"

uint8_t RxData[256];
uint8_t TxData[256];
//extern ScreenMain_t* ScreenMain;
extern Control_motor_t* Control_motor;
extern Cylinder_and_save_t* Cylinder_and_save;
extern Rubber_and_tray_t* Rubber_and_tray;


extern Axis_t AxisX, AxisY, AxisZ;
uint32_t test_builtin;
extern uint16_t Input_Registers_Database[50];
extern uint16_t* Mark;
extern const uint32_t FlashStart;


ActionHandler_t motormoveTable[] =  {
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


void application_run(){
	Control_motor_z();
	Control_motor_y();
	Control_motor_x();
}
void task_timer6(){
	test_builtin = __builtin_ffs(Rubber_and_tray->all << 16 | Cylinder_and_save->all<< 8 | Control_motor->all);

	if (test_builtin > 0) {
	    test_builtin -= 1;

	    if (test_builtin < (int)(sizeof(motormoveTable) / sizeof(motormoveTable[0]))) {
	        motormoveTable[test_builtin]();
	    }
	}
	Handle(24);
}
void application_run_main(void){

}

