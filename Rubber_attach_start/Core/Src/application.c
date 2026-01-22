/*
 * application.c
 *
 *  Created on: Jan 3, 2026
 *      Author: Admin
 */

#include "application.h"

extern uint8_t RxData[256];
extern uint8_t TxData[256];

extern Control_motor_t* Control_motor;
extern Cylinder_and_save_t* Cylinder_and_save;
extern Rubber_and_tray_t* Rubber_and_tray;


extern Axis_t AxisX, AxisY, AxisZ;
extern uint16_t Input_Registers_Database[50];
extern uint8_t Inputs_Database[50];
extern uint16_t* Mark;
extern const uint32_t FlashStart;
extern uint8_t SS_Door_Left;
extern uint8_t SS_Door_Right;
extern volatile uint8_t state_door;

Taskbar_t* Taskbar = (Taskbar_t*)&Coils_Database[0];
Tab_main_t* Tab_main = (Tab_main_t*)&Coils_Database[5];
Tab_popup_t* Tab_popup = (Tab_popup_t*)&Coils_Database[6];

Tab_main_t* Tab_main_indicator = (Tab_main_t*) &Inputs_Database[0];
Tab_popup_t* Tab_popup_indicator = (Tab_popup_t*) &Inputs_Database[35];

volatile uint8_t flag_Stop = 0;


ActionHandler_t Tab_main_table[] =  {
		 Handle_reset,
		 Handle_start,
		 Handle_stop,
};

ActionHandler_t Tab_popup_table[] =  {
		 Popup_handle_stop,
		 Popup_handle_next,
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
		HAL_Delay(6000);
		Mark_all_rubber();
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxData, 256);
		Taskbar->bits.home = 1 ;
		Read_Tray_Data();
		HAL_TIM_Base_Start_IT(&htim5); //x
		HAL_TIM_Base_Start_IT(&htim9); //y
		HAL_TIM_Base_Start_IT(&htim2); //z

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
	while(DOOR_OPEN()){
		Open_Popup(1);
	}
	Close_Popup(1);
	Open_Popup_home();
	// HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	 Cylinder1_Go_Up;
	 Cylinder2_Go_Up;
	  if(get_home_z() == home_z){
		  AxisZ.mode = MOVE_HOME2;
	  }else {
		  AxisZ.mode = MOVE_HOME1;
	  }
		while((AxisZ.mode != MOVE_HOME2));

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
//	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	  HAL_TIM_Base_Start_IT(&htim6);
	  Close_Popup_home();
}

void Handle_popup(void){
	uint8_t builtin_Handle_popup = __builtin_ffs(Tab_popup->all);
		if (builtin_Handle_popup > 0) {
			builtin_Handle_popup -= 1;
		    if (builtin_Handle_popup < (int)(sizeof(Tab_popup_table) / sizeof(Tab_popup_table[0]))) {
		    	Tab_popup_table[builtin_Handle_popup]();
		    }
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
	Tab_main_indicator->bits.reset =  1 ;
}

void Handle_start(void){
	Tab_main_indicator->bits.start =  1 ;
	Handle();

}
void Handle_stop(void){
	Tab_main_indicator->bits.stop =  1 ;
}

void Popup_handle_next(void){
	//Tab_popup->bits.next = 0;
	//st_continue = 1;
	//Close_Popup(0);
}

void Popup_handle_stop(void){
	//Tab_popup->bits.stop = 0;
	//st_stop = 1;
	//Close_Popup(0);
}

void task_timer6(){
//	if(!DOOR_OPEN()){
		if(Taskbar->bits.home){
			Handle_main();
		}else if(Taskbar->bits.motor){
			Handle_motor();
		}
		Handle_popup();
//	}

//	else if(Taskbar->bits.SETTING == 1){
//
//	}
}
void task_timer7(){
	Control_motor_y();
	Control_motor_x();
	Control_motor_z();
	BipControl();
//	if(DOOR_OPEN()){
//		Open_Popup(1);
//	}
//	else Close_Popup(1);
}


void application_run_main(void){
	  if(Timer_Check(0, 500) && !DOOR_OPEN()){
		  OFF_LED_RED;
		  OFF_BUZZ;
		  TOGGLE_LED_GREEN;
	  }
	  else if(Timer_Check(2, 500) && DOOR_OPEN()){
		  OFF_LED_GREEN;
		  TOGGLE_LED_RED;
		  TOGGLE_BUZZ;
	  }
//	  else if(state_door == 1){
//		  if(Tab_main->bits.start == 1){
//			  state_door = 10;
//			  HAL_TIM_Base_Start_IT(&htim6);
//			  HAL_TIM_Base_Start_IT(&htim7);
//		  }
//	  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == i16_start_Pin){
		if(HAL_GPIO_ReadPin(i16_start_GPIO_Port, i16_start_Pin)){
			if(state_door == 1){
				if(Tab_main_indicator->bits.start == 0 ){
				AxisX.mode =  AxisX.pre_mode;
				AxisY.mode =  AxisY.pre_mode;
				AxisZ.mode =  AxisZ.pre_mode;
				AxisX.pre_mode = STOP;
				AxisY.pre_mode = STOP;
				AxisZ.pre_mode = STOP;
				  HAL_TIM_Base_Start_IT(&htim6);
				  HAL_TIM_Base_Start_IT(&htim7);
				  state_door = 10;
				  return ;
					}else{
						  HAL_TIM_Base_Start_IT(&htim6);
						  HAL_TIM_Base_Start_IT(&htim7);
						  state_door = 10;
						  return;
					}
				}

			if((AxisX.mode == STOP) &&(AxisY.mode == STOP)&&(AxisZ.mode == STOP) ){
			Tab_main->bits.start = 1;
			}
			if(Taskbar->bits.motor ==  1){
				Tab_main->bits.start = 0;
			}


		}
	}
	else if(GPIO_Pin == i4_estop_Pin){
		if(HAL_GPIO_ReadPin(i4_estop_GPIO_Port, i4_estop_Pin)){
			NVIC_SystemReset();
		}
	}
	else if(GPIO_Pin == i5_stop_Pin){
		if(HAL_GPIO_ReadPin(i5_stop_GPIO_Port, i5_stop_Pin)){ // mo
			flag_Stop = 1;
		}else{

		}
	}
//	else if(GPIO_Pin == i17_reset_Pin){
//
//	}
//	else if(GPIO_Pin ==  i14_Door_L_Pin){
//		//dong
////		if(HAL_GPIO_ReadPin(i14_Door_L_GPIO_Port, i14_Door_L_Pin)){
////			HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin,SET);
////		//mo
////		}else{
////			HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, RESET);
////			if(AxisX.mode == MOVE_AUTO){
////				AxisX.mode = MOVE_MANUAL;
////			}
////			if(AxisY.mode == MOVE_AUTO){
////				AxisY.mode = MOVE_MANUAL;
////			}
////			if(AxisZ.mode == MOVE_AUTO){
////				AxisZ.mode = MOVE_MANUAL;
////			}
////				Stop_motor_x();
////				Stop_motor_y();
////				Stop_motor_z();
////		}
//	}
}

