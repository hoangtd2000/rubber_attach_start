/*
 * application.c
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#include "application.h"
uint8_t RxData[256];
uint8_t TxData[256];
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
		//HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxData, 256);


		uint32_t data[10];
		Flash_Read_Data( FlashStart, data, 10);

		for (uint8_t i = 0; i < 9; i++)
		{
		    Mark[2*i]     =  data[i]        & 0xFFFF;        // low 16-bit
		    Mark[2*i + 1] = (data[i] >> 16) & 0xFFFF;        // high 16-bit
		}
		HAL_Delay(10000);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxData, 256);
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
}




void application_run_main(void){
//	for(int i = 1 ; i <= 200; i++){
//		Input_Registers_Database[0] = i;
//		HAL_Delay(300);
//	}
//	for(int j = 1 ; j <= 24; j++){
//		Input_Registers_Database[1] = j;
//		HAL_Delay(300);
//	}
//	for(int g = 1 ; g <= 24; g++){
//		Input_Registers_Database[2] = g;
//		HAL_Delay(300);
//	}
//	  HAL_GPIO_TogglePin(O1_GPIO_Port, O1_Pin);
//		HAL_Delay(500);
//	  HAL_GPIO_TogglePin(O2_GPIO_Port, O2_Pin);
//		HAL_Delay(500);
//	HAL_GPIO_TogglePin(O3_GPIO_Port, O3_Pin);
//	HAL_Delay(500);
//	HAL_GPIO_TogglePin(O4_GPIO_Port, O4_Pin);
//	HAL_Delay(500);
//	HAL_GPIO_TogglePin(O5_GPIO_Port, O5_Pin);
//	HAL_Delay(500);
//	HAL_GPIO_TogglePin(O6_GPIO_Port, O6_Pin);
//	HAL_Delay(500);
//		  HAL_GPIO_TogglePin(O10_GPIO_Port, O10_Pin);
//		  HAL_GPIO_TogglePin(O11_GPIO_Port, O11_Pin);
//		  HAL_GPIO_TogglePin(O12_GPIO_Port, O12_Pin);
//		  HAL_Delay(500);



}


