/*
 * motor_control.c
 *
 *  Created on: Oct 17, 2025
 *      Author: TranHoang
 */

#include "motor_control.h"
Control_motor_t* Control_motor = (Control_motor_t*)&Coils_Database[1];
Save_Tray_t* Save_Tray = (Save_Tray_t*)&Coils_Database[4];

//Position_t Current_t = {0,0,0};
//Position_t Old_t = {0,0,0};
Axis_t AxisX = {
		.dir =  DIR_POS,
		.is_moving = 0 ,
		.mode = STOP,
		.pulse_count = 0 ,
		.stop_request = 0 ,
		.target_pulse = 0,
		.current_pos = 0 ,
		.old_pos = 0,
		.speed = 100
};

Axis_t AxisY = {
		.dir =  DIR_POS,
		.is_moving = 0 ,
		.mode = STOP,
		.pulse_count = 0 ,
		.stop_request = 0 ,
		.current_pos = 0 ,
		.old_pos = 0,
		.target_pulse = 0
};
Axis_t AxisZ = {
		.dir =  DIR_POS,
		.is_moving = 0 ,
		.mode = STOP,
		.pulse_count = 0 ,
		.stop_request = 0 ,
		.current_pos = 0 ,
		.old_pos = 0,
		.target_pulse = 0
};


void Set_Speed_Motor_x(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	set_speed_x(2000000/f);
	set_pulse_x(2000000/f/2);
//	htim8.Instance->ARR = 2000000/f;
//	htim8.Instance->CCR1 = htim8.Instance->ARR / 2;
}
void Set_Speed_Motor_y(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	set_speed_y(2000000/f);
	set_pulse_y(2000000/f/2);
//	htim3.Instance->ARR = 2000000/f;
//	htim3.Instance->CCR3 =  htim3.Instance->ARR / 2;
}
void Set_Speed_Motor_z(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	set_speed_z(1000000/f);
	set_pulse_z(1000000/f/2);
//	htim1.Instance->ARR = 2000000/f;
//	htim1.Instance->CCR1 =htim1.Instance->ARR / 2;
}


void excute_move_x_left(void){
	output_x_sig_left();
	output_x_pull_start();
}
void excute_move_x_right(void){
	output_x_sig_right();
	output_x_pull_start();
}
void excute_move_y_forward(void){
	output_y_sig_forward();
	output_y_pull_start();
}
void excute_move_y_backward(void){
	output_y_sig_backward();
	output_y_pull_start();
}
void excute_move_z_up(void){
	output_z_sig_up();
	output_z_pull_start();
}
void excute_move_z_down(void){
	output_z_sig_down();
	output_z_pull_start();
}


void move_x_left(uint16_t d){
	AxisX.dir = DIR_NEG;
	//AxisX.is_moving = 1 ;
//	AxisX.pulse_count = 1 ;
//	AxisX.target_pulse = (d)  ;
//	set_x_counter(0);
	reset_counter_timer_x();
//	reset_counter_timer_slave_x();
	set_x_target_pull(d-1);
//	reset_counter_timer_x();
	excute_move_x_left();
}


void move_x_right(uint16_t d){
	AxisX.dir = DIR_POS;
	//AxisX.is_moving = 1 ;
//	AxisX.pulse_count = 1 ;
//	AxisX.target_pulse =  (d) ;
//	set_x_counter(0);
	reset_counter_timer_x();
	//reset_counter_timer_slave_x();
	set_x_target_pull(d-1);
//	reset_counter_timer_x();
	excute_move_x_right();
}

void move_y_forward(uint16_t d){
	AxisY.dir = DIR_POS;
//	AxisY.is_moving = 1 ;
//	AxisY.pulse_count = 1 ;
//	AxisY.target_pulse = (d)  ;
	reset_counter_timer_y();
	set_y_target_pull(d-1);
	excute_move_y_forward();
}

void move_y_backward(uint16_t d){
	AxisY.dir = DIR_NEG;
//	AxisY.is_moving = 1 ;
//	AxisY.pulse_count = 1 ;
//	AxisY.target_pulse = (d)  ;
	reset_counter_timer_y();
	set_y_target_pull(d-1);
	excute_move_y_backward();
}

void move_z_up(uint16_t d){
	AxisZ.dir = DIR_NEG;
//	AxisZ.is_moving = 1 ;
//	AxisZ.pulse_count = 1 ;
//	AxisZ.target_pulse = (d)  ;
	reset_counter_timer_z();
	set_z_target_pull(d-1);
	excute_move_z_up();
}

void move_z_down(uint16_t d){
	AxisZ.dir = DIR_POS;
//	AxisZ.is_moving = 1 ;
//	AxisZ.pulse_count = 1 ;
//	AxisZ.target_pulse = (d)  ;
	reset_counter_timer_z();
	set_z_target_pull(d-1);
	excute_move_z_down();
}

void move_axis(uint16_t xd, uint16_t yd, uint16_t zd){
    if(xd > max_x || yd > max_y || zd > max_z ){
        return;
    }
    if(AxisX.current_pos != xd ){
    	Set_Speed_Motor_x( speed_run, speed_x_max);
    	AxisX.mode = MOVE_AUTO;
    	if(AxisX.current_pos > xd){
    		move_x_left(AxisX.current_pos - xd  );
    	}else{
    		move_x_right(xd- AxisX.current_pos );
    	}
    }
    if(AxisY.current_pos != yd ){
	Set_Speed_Motor_y( speed_run, speed_y_max);
   	AxisY.mode = MOVE_AUTO;
    	if(AxisY.current_pos > yd){
    		move_y_backward(AxisY.current_pos - yd );
    	}else{
    		move_y_forward(yd-AxisY.current_pos );
    	}
    }
    if(AxisZ.current_pos != zd ){
	Set_Speed_Motor_z( speed_run_z, speed_z_max);
   	AxisZ.mode = MOVE_AUTO;
    	if(AxisZ.current_pos > zd){
    		move_z_up(AxisZ.current_pos -  zd );
    	}else{
    		move_z_down(zd - AxisZ.current_pos );
    	}
    }
}



 void Set_HMI_X_Axis(uint16_t value){
	Holding_Registers_Database[0] = value;
}
 void Set_HMI_Y_Axis(uint16_t value){
	Holding_Registers_Database[1] = value;
}
void Set_HMI_Z_Axis(uint16_t value){
	Holding_Registers_Database[2] = value;
}

uint16_t Get_HMI_X_Axis(void){
	return Holding_Registers_Database[0];
}
uint16_t Get_HMI_Y_Axis(void){
	return Holding_Registers_Database[1];
}
uint16_t Get_HMI_Z_Axis(void){
	return Holding_Registers_Database[2];
}


uint8_t get_coil(uint16_t reg_addr)
{
    if (reg_addr == 0 || reg_addr > 200)
        return 0;

    uint16_t bit_index = reg_addr - 1;
    uint16_t byte_index = bit_index / 8;
    uint8_t  bit_pos    = bit_index % 8;

    return (Coils_Database[byte_index] >> bit_pos) & 0x01;
}


void Handle_X_Left (void){
//	HAL_GPIO_TogglePin(O1_GPIO_Port, O1_Pin);
	if(AxisX.current_pos <= 0) {
		return;
		}
	if(AxisX.mode == STOP) {
		AxisX.mode =  MOVE_MANUAL;
		move_x_left(AxisX.current_pos);
		}
}
void Handle_X_Right (void){
	//  HAL_GPIO_TogglePin(O2_GPIO_Port, O2_Pin);
	if(AxisX.current_pos >= max_x){
		return;
	}
	if(AxisX.mode == STOP) {
	  AxisX.mode =  MOVE_MANUAL;
	  move_x_right(max_x - AxisX.current_pos);
	}

}
void Handle_Y_Forward(void){
//	HAL_GPIO_TogglePin(O3_GPIO_Port, O3_Pin);
	if(AxisY.current_pos >= max_y) {
		return;
		}
	if(AxisY.mode == STOP){
		AxisY.mode = MOVE_MANUAL;
		move_y_forward(max_y- AxisY.current_pos);
	}
}
void Handle_Y_Backward(void){
//	 HAL_GPIO_TogglePin(O4_GPIO_Port, O4_Pin);
	if(AxisY.current_pos <= 0  ){
		return ;
	}
	if(AxisY.mode == STOP){
		AxisY.mode = MOVE_MANUAL;
		move_y_backward( AxisY.current_pos);
	}
}
void Handle_Z_Up(void){
//	 HAL_GPIO_TogglePin(O5_GPIO_Port, O5_Pin);
	if(AxisZ.current_pos <=0  ){
			return ;
		}
	if(AxisZ.mode == STOP) {
		AxisZ.mode = MOVE_MANUAL;
		move_z_up(AxisZ.current_pos);
	}
}
void Handle_Z_Down(void){
//	HAL_GPIO_TogglePin(O6_GPIO_Port, O6_Pin);
	if(AxisZ.current_pos >=max_z  ){
			return ;
		}
	if(AxisZ.mode == STOP) {
		AxisZ.mode = MOVE_MANUAL;
		move_z_down(max_z-AxisZ.current_pos);
	}

}

void Handle_Set(void){
	//HAL_GPIO_TogglePin(O7_GPIO_Port, O7_Pin);
	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
	move_axis(Get_HMI_X_Axis(),Get_HMI_Y_Axis(), Get_HMI_Z_Axis());
	}
}
void Handle_Home(void){
	//HAL_GPIO_TogglePin(O8_GPIO_Port, O8_Pin);
	if(get_home_x() != home_x){
		if(AxisX.mode == STOP ){
			if(AxisX.current_pos >0){
				AxisX.mode = MOVE_HOME1;
				}
			}
	}
	if(get_home_y() != home_y){
		if(AxisY.mode == STOP ){
			if(AxisY.current_pos > 0){
			AxisY.mode = MOVE_HOME1;
				}
			}
	}
	if(get_home_z() != home_z){
		if(AxisZ.mode == STOP ){
			if(AxisZ.current_pos >0){
			AxisZ.mode = MOVE_HOME1;
				}
			}
	}
}




void Control_motor_x(){
	switch(AxisX.mode) {
	case MOVE_AUTO:
		AxisX.current_pos = AxisX.old_pos + (AxisX.dir*( get_counter_timer_slave_x() ) );
		Set_HMI_X_Axis(AxisX.current_pos);
		break;
	case MOVE_MANUAL:
		if((Control_motor->bits.x_Left == 0 ) && (Control_motor->bits.x_Right == 0 )){
			Stop_motor_x();
		}
		AxisX.current_pos = AxisX.old_pos + (AxisX.dir*( get_counter_timer_slave_x()  ) );
		Set_HMI_X_Axis(AxisX.current_pos);
		Set_Speed_Motor_x( (get_counter_timer_slave_x()) + speed_default, speed_x_max);
		break;
	case MOVE_HOME1:
		Set_Speed_Motor_x(speed_home1_x, speed_x_max);
		move_x_left(max_x+500);
		break;
	case MOVE_HOME2:
		if( get_counter_timer_slave_x() == 0 ){
		Set_Speed_Motor_x(speed_home2_x, speed_x_max);
		move_x_right(2000);
		}
		break;
	case MOVE_HOME3:
		Set_Speed_Motor_x(speed_home3_x, speed_x_max);
		move_x_left(max_x);
		break;
	case STOP:

		break;
	}
}
void Stop_motor_x(void){
	output_x_pull_stop();
	if(AxisX.mode == MOVE_MANUAL){
		AxisX.current_pos = AxisX.old_pos + (AxisX.dir*( get_counter_timer_slave_x() ) );
		Set_HMI_X_Axis(AxisX.current_pos);
	reset_counter_timer_slave_x();
	}
	else if(AxisX.mode == MOVE_AUTO){
		AxisX.current_pos = AxisX.old_pos + (AxisX.dir*(get_x_target_pull()+1));
		Set_HMI_X_Axis(AxisX.current_pos);
	}else if(AxisX.mode == MOVE_HOME2){
		AxisX.mode = MOVE_HOME3;
		return;
	}
	AxisX.mode = STOP;
	AxisX.old_pos = AxisX.current_pos;
}
void Control_motor_y(){
	switch(AxisY.mode) {
	case MOVE_AUTO:
		AxisY.current_pos = AxisY.old_pos + (AxisY.dir*( get_counter_timer_slave_y()  ) );
		Set_HMI_Y_Axis(AxisY.current_pos);
		break;
	case MOVE_MANUAL:
		if((Control_motor->bits.y_Forward == 0 ) && (Control_motor->bits.y_Backward == 0 )){
					Stop_motor_y();
				}
		AxisY.current_pos = AxisY.old_pos + (AxisY.dir*( get_counter_timer_slave_y()  ) );
				Set_HMI_Y_Axis(AxisY.current_pos);
				Set_Speed_Motor_y( (get_counter_timer_slave_y()) + speed_default, speed_y_max);
		break;
	case MOVE_HOME1:
		Set_Speed_Motor_y(speed_home1_y, speed_y_max);
		move_y_backward(max_y + 500);
		break;
	case MOVE_HOME2:
		if( get_counter_timer_slave_y() == 0 ){
		Set_Speed_Motor_y(speed_home2_y, speed_y_max);
		move_y_forward(2000);
		}
		break;
	case MOVE_HOME3:
		Set_Speed_Motor_y(speed_home3_y, speed_y_max);
		move_y_backward(max_y );
		break;
	case STOP:
		break;
	}
}
void Control_motor_z(){
	switch(AxisZ.mode) {
	case MOVE_AUTO:
		AxisZ.current_pos = AxisZ.old_pos + (AxisZ.dir*(get_counter_timer_slave_z() ) );
				Set_HMI_Z_Axis(AxisZ.current_pos);
		break;
	case MOVE_MANUAL:

		if((Control_motor->bits.z_Up == 0 ) && (Control_motor->bits.z_Down == 0 )){
					Stop_motor_z();
				}
		AxisZ.current_pos = AxisZ.old_pos + (AxisZ.dir*(get_counter_timer_slave_z() ) );
				Set_HMI_Z_Axis(AxisZ.current_pos);
				Set_Speed_Motor_z((get_counter_timer_slave_z()) + speed_default, speed_z_max);
		break;
	case MOVE_HOME1:
		Set_Speed_Motor_z(speed_home1_z, speed_z_max);
		// cộng thêm tránh trường hợp đi từ max vào
		move_z_up(max_z + 1000);
		break;
	case MOVE_HOME2:
		if( get_counter_timer_slave_z() == 0 ){
		Set_Speed_Motor_z(speed_home2_z, speed_z_max);
		move_z_down(2000);
		}
		break;
	case MOVE_HOME3:
		Set_Speed_Motor_z(speed_home3_z, speed_z_max);
		move_z_up(max_z);
		break;
	case STOP:

		break;
	}
}

void Stop_motor_y(void){
	output_y_pull_stop();
		if(AxisY.mode == MOVE_MANUAL){
			AxisY.current_pos = AxisY.old_pos + (AxisY.dir*(get_counter_timer_slave_y()) );
			Set_HMI_Y_Axis(AxisY.current_pos);
		reset_counter_timer_slave_y();
		}
		else if(AxisY.mode == MOVE_AUTO){
			AxisY.current_pos = AxisY.old_pos + (AxisY.dir*(get_y_target_pull() + 1));
			Set_HMI_Y_Axis(AxisY.current_pos);
		}else if(AxisY.mode == MOVE_HOME2){
			AxisY.mode = MOVE_HOME3;
			return;
		}
		AxisY.mode = STOP;
		AxisY.old_pos = AxisY.current_pos;
}
void Stop_motor_z(void){
	output_z_pull_stop();
		if(AxisZ.mode == MOVE_MANUAL){
			AxisZ.current_pos = AxisZ.old_pos + (AxisZ.dir*(get_counter_timer_slave_z()) );
			Set_HMI_Z_Axis(AxisZ.current_pos);
		reset_counter_timer_slave_z();
		}
		else if(AxisZ.mode == MOVE_AUTO){
			AxisZ.current_pos = AxisZ.old_pos + (AxisZ.dir*(get_z_target_pull() + 1)  );
			Set_HMI_Z_Axis(AxisZ.current_pos);
		}else if(AxisZ.mode == MOVE_HOME2){
			AxisZ.mode = MOVE_HOME3;
			return;
		}
		AxisZ.mode = STOP;
		AxisZ.old_pos = AxisZ.current_pos;
}
