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
extern Axis_t AxisX, AxisY, AxisZ;
uint16_t test_builtin;
ActionHandler_t motormoveTable[] =  {
		 Handle_X_Left,
		 Handle_X_Right,
		 Handle_Y_Backward,
		 Handle_Y_Forward,
		 Handle_Z_Up,
		 Handle_Z_Down,
		 Handle_Set,
		 Handle_Home,
};


void application_init(){
		HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxData, 256);
		HAL_Delay(5000);
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
	test_builtin = __builtin_ffs(Coils_Database[1]);

	if (test_builtin > 0) {
	    test_builtin -= 1;

	    if (test_builtin < (int)(sizeof(motormoveTable) / sizeof(motormoveTable[0]))) {
	        motormoveTable[test_builtin]();
	    }
	}
}
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//
//	 if(GPIO_Pin == i1_home_x_Pin){
//	  if(get_home_x() == home_x){
//		//  HAL_GPIO_TogglePin(O1_GPIO_Port, O1_Pin);
//		  switch(AxisX.mode){
//		  case MOVE_HOME1:
//			  			output_x_pull_stop();
//			  		  reset_counter_timer_slave_x();
//			  			AxisX.current_pos = 0;
//			  			Set_HMI_X_Axis(AxisX.current_pos);
//			  		  AxisX.mode = MOVE_HOME2;
//			  		AxisX.old_pos = 0;
//			  break;
//		  case MOVE_HOME3:
//			  			output_x_pull_stop();
//			  		  reset_counter_timer_slave_x();
//			  			AxisX.current_pos = 0;
//			  			Set_HMI_X_Axis(AxisX.current_pos);
//			  			AxisX.mode  = STOP;
//			  			AxisX.old_pos = 0;
//			  break;
//		  }
//	  }
//	 }
//	 if(GPIO_Pin == i2_home_y_Pin){
//	  if(get_home_y() == home_y){
//		//  HAL_GPIO_TogglePin(O2_GPIO_Port, O2_Pin);
//		  switch(AxisY.mode){
//		  case MOVE_HOME1:
//			  			output_y_pull_stop();
//			  		  reset_counter_timer_slave_y();
//			  			AxisY.current_pos = 0;
//			  			Set_HMI_Y_Axis(AxisY.current_pos);
//			  		  AxisY.mode = MOVE_HOME2;
//			  		AxisY.old_pos = 0;
//			  break;
//		  case MOVE_HOME3:
//			  			output_y_pull_stop();
//			  		  reset_counter_timer_slave_y();
//			  			AxisY.current_pos = 0;
//			  			Set_HMI_Y_Axis(AxisY.current_pos);
//			  			AxisY.mode  = STOP;
//			  			AxisY.old_pos = 0;
//			  break;
//		  }
//	  }
//	 }
////	 if(GPIO_Pin == i3_home_z_Pin){
////	  if(get_home_z() == home_z){
////		  HAL_GPIO_TogglePin(O3_GPIO_Port, O3_Pin);
////		  switch(AxisZ.mode){
////		  case MOVE_HOME1:
////			  			output_z_pull_stop();
////			  		  reset_counter_timer_slave_z();
////			  			AxisZ.current_pos = 0;
////			  			Set_HMI_Z_Axis(AxisZ.current_pos);
////			  		  AxisZ.mode = MOVE_HOME2;
////			  		AxisZ.old_pos = 0;
////			  break;
////		  case MOVE_HOME3:
////			  			output_z_pull_stop();
////			  		  reset_counter_timer_slave_z();
////			  			AxisZ.current_pos = 0;
////			  			Set_HMI_Z_Axis(AxisZ.current_pos);
////			  			AxisZ.mode  = STOP;
////			  			AxisZ.old_pos = 0;
////			  break;
////		  }
////	  }
////	 }
//}

