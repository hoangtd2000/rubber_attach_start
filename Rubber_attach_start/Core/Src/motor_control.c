/*
 * motor_control.c
 *
 *  Created on: Oct 17, 2025
 *      Author: TranHoang
 */

#include "motor_control.h"
#include "IO_Controller.h"

#define truc_z_len_het
#define truc_z_len_sau

Control_motor_t* Control_motor = (Control_motor_t*)&Coils_Database[1];
Cylinder_and_save_t* Cylinder_and_save = (Cylinder_and_save_t*)&Coils_Database[2];
Rubber_and_tray_t* Rubber_and_tray  = (Rubber_and_tray_t*)&Coils_Database[3];
Rubber_and_tray_indicator_t* Rubber_and_tray_indicator = (Rubber_and_tray_indicator_t*)&Inputs_Database[1];

Control_Vacum_Indicator_t* Control_Vacum_Indicator = (Control_Vacum_Indicator_t*)&Inputs_Database[2];

Point2D Rubber_Mark[3];
Point2D Tray1_Mark[3];
Point2D Tray2_Mark[3];

extern uint8_t Coils_Database[25];
extern uint16_t Holding_Registers_Database[300];
extern uint8_t Inputs_Database[50];
extern Item Rubber_Tray[400] ;
extern Item Tray1[30];
extern Item Tray2[30];



uint16_t* Mark = &Holding_Registers_Database[6];
const uint32_t FlashStart = 0x0800C000;

 uint32_t data[10];

ActionHandler_t Move_tray_table[] =  {
		 Move_tray_rubber_p1,
		 Move_tray_rubber_p2,
		 Move_tray_rubber_p3,
		 Move_tray1_p1,
		 Move_tray1_p2,
		 Move_tray1_p3,
		 Move_tray2_p1,
		 Move_tray2_p2,
		 Move_tray2_p3,
};
Axis_t AxisX = {
		.dir =  DIR_POS,
		.mode = STOP,
		.pre_mode = STOP,
		.current_pos = 0 ,
		.old_pos = 0,
};

Axis_t AxisY = {
		.dir =  DIR_POS,
		.mode = STOP,
		.pre_mode = STOP,
		.current_pos = 0 ,
		.old_pos = 0,
};
Axis_t AxisZ = {
		.dir =  DIR_POS,
		.mode = STOP,
		.pre_mode = STOP,
		.current_pos = 0 ,
		.old_pos = 0,
};


void Set_Speed_Motor_x(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	set_speed_x(2000000/f);
	set_pulse_x(2000000/f/2);
}
void Set_Speed_Motor_y(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	set_speed_y(2000000/f);
	set_pulse_y(2000000/f/2);
}
void Set_Speed_Motor_z(uint16_t f, uint16_t f_max){
	if(f > f_max) f = f_max;
	set_speed_z(1000000/f);
	set_pulse_z(1000000/f/2);
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
	reset_counter_timer_x();
	set_x_target_pull(d-1);
	excute_move_x_left();
}


void move_x_right(uint16_t d){
	AxisX.dir = DIR_POS;
	reset_counter_timer_x();
	set_x_target_pull(d-1);
	excute_move_x_right();
}

void move_y_forward(uint16_t d){
	AxisY.dir = DIR_POS;
	reset_counter_timer_y();
	set_y_target_pull(d-1);
	excute_move_y_forward();
}

void move_y_backward(uint16_t d){
	AxisY.dir = DIR_NEG;
	reset_counter_timer_y();
	set_y_target_pull(d-1);
	excute_move_y_backward();
}

void move_z_up(uint16_t d){
	AxisZ.dir = DIR_NEG;
	reset_counter_timer_z();
	set_z_target_pull(d-1);
	excute_move_z_up();
}

void move_z_down(uint16_t d){
	AxisZ.dir = DIR_POS;
	reset_counter_timer_z();
	set_z_target_pull(d-1);
	excute_move_z_down();
}

static inline uint16_t abs_diff_u16(uint16_t a, uint16_t b)
{
    return (a > b) ? (a - b) : (b - a);
}

// move z -> x,y
void move_axis(uint16_t xd, uint16_t yd, uint16_t zd)
{
    /* Check giới hạn */
    if (xd > max_x || yd > max_y || zd > max_z) {
        return;
    }
    /* ================= Z AXIS ================= */
    uint16_t dz = abs_diff_u16(AxisZ.current_pos, zd);
    if (dz > 1 && AxisZ.mode == STOP) {
        Set_Speed_Motor_z(speed_run_z, speed_z_max);
        AxisZ.mode = MOVE_AUTO;

        if (AxisZ.current_pos > zd) {
            move_z_up(dz);
        } else {
            move_z_down(dz);
        }
    }
#ifdef truc_z_len_het
    while(AxisZ.mode != STOP);
#endif
    /* ================= X AXIS ================= */
    uint16_t dx = abs_diff_u16(AxisX.current_pos, xd);
    if (dx > 1 && AxisX.mode == STOP) {
        Set_Speed_Motor_x(speed_run, speed_x_max);
        AxisX.mode = MOVE_AUTO;

        if (AxisX.current_pos > xd) {
            move_x_left(dx);
        } else {
            move_x_right(dx);
        }
    }

    /* ================= Y AXIS ================= */
    uint16_t dy = abs_diff_u16(AxisY.current_pos, yd);
    if (dy > 1 && AxisY.mode == STOP) {
        Set_Speed_Motor_y(speed_run, speed_y_max);
        AxisY.mode = MOVE_AUTO;

        if (AxisY.current_pos > yd) {
            move_y_backward(dy);
        } else {
            move_y_forward(dy);
        }
    }
}

// move x,y -> z
void move_axis1(uint16_t xd, uint16_t yd, uint16_t zd)
{
    /* Check giới hạn */
    if (xd > max_x || yd > max_y || zd > max_z) {
        return;
    }

    /* ================= X AXIS ================= */
    uint16_t dx = abs_diff_u16(AxisX.current_pos, xd);
    if (dx > 1 && AxisX.mode == STOP) {
        Set_Speed_Motor_x(speed_run, speed_x_max);
        AxisX.mode = MOVE_AUTO;

        if (AxisX.current_pos > xd) {
            move_x_left(dx);
        } else {
            move_x_right(dx);
        }
    }

    /* ================= Y AXIS ================= */
    uint16_t dy = abs_diff_u16(AxisY.current_pos, yd);
    if (dy > 1 && AxisY.mode == STOP) {
        Set_Speed_Motor_y(speed_run, speed_y_max);
        AxisY.mode = MOVE_AUTO;

        if (AxisY.current_pos > yd) {
            move_y_backward(dy);
        } else {
            move_y_forward(dy);
        }
    }
    while ( !(AxisX.mode == STOP && AxisY.mode == STOP ));

    /* ================= Z AXIS ================= */
    uint16_t dz = abs_diff_u16(AxisZ.current_pos, zd);
    if (dz > 1 && AxisZ.mode == STOP) {
        Set_Speed_Motor_z(speed_run_z, speed_z_max);
        AxisZ.mode = MOVE_AUTO;

        if (AxisZ.current_pos > zd) {
            move_z_up(dz);
        } else {
            move_z_down(dz);
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
		AxisX.current_pos = AxisX.old_pos + (AxisX.dir*( get_counter_timer_slave_x()  ) );
		Set_HMI_X_Axis(AxisX.current_pos);
		Set_Speed_Motor_x( (get_counter_timer_slave_x()) + speed_default, speed_x_max);
		if((Control_motor->bits.x_Left == 0 ) && (Control_motor->bits.x_Right == 0 )){
			if(get_counter_timer_x() <= get_pulse_x() ){
			Stop_motor_x();
			}
		}
		break;
	case MOVE_HOME1:
		Set_Speed_Motor_x(speed_home1_x, speed_x_max);
		move_x_left(max_x+1000);
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

void Control_motor_y(){
	switch(AxisY.mode) {
	case MOVE_AUTO:
		AxisY.current_pos = AxisY.old_pos + (AxisY.dir*( get_counter_timer_slave_y()  ) );
		Set_HMI_Y_Axis(AxisY.current_pos);
		break;
	case MOVE_MANUAL:
		AxisY.current_pos = AxisY.old_pos + (AxisY.dir*( get_counter_timer_slave_y()  ) );
				Set_HMI_Y_Axis(AxisY.current_pos);
				Set_Speed_Motor_y( (get_counter_timer_slave_y()) + speed_default, speed_y_max);
		if((Control_motor->bits.y_Forward == 0 ) && (Control_motor->bits.y_Backward == 0 )){
			if(get_counter_timer_y() <= get_pulse_y() ){
					Stop_motor_y();
					}
				}
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

		AxisZ.current_pos = AxisZ.old_pos + (AxisZ.dir*(get_counter_timer_slave_z() ) );
				Set_HMI_Z_Axis(AxisZ.current_pos);
				Set_Speed_Motor_z((get_counter_timer_slave_z()) + speed_default, speed_z_max);
		if((Control_motor->bits.z_Up == 0 ) && (Control_motor->bits.z_Down == 0 )){
			if(get_counter_timer_z() < get_pulse_z() ){
				Stop_motor_z();
					}
				}
		break;
	case MOVE_HOME1:
		Set_Speed_Motor_z(speed_home1_z, speed_z_max);
		// cộng thêm tránh trường hợp đi từ max vào
		move_z_up(max_z + 1000);
		break;
	case MOVE_HOME2:
		if( get_counter_timer_slave_z() == 0 ){
		Set_Speed_Motor_z(speed_home2_z, speed_z_max);
		move_z_down(2500);
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
void Stop_motor_x(void)
{
    output_x_pull_stop();
    switch (AxisX.mode) {
    case MOVE_MANUAL:
        AxisX.current_pos =
            AxisX.old_pos + (AxisX.dir * get_counter_timer_slave_x());
        Set_HMI_X_Axis(AxisX.current_pos);
        reset_counter_timer_slave_x();
        break;

    case MOVE_AUTO:
        AxisX.current_pos =
            AxisX.old_pos + (AxisX.dir * (get_x_target_pull() + 1));
        Set_HMI_X_Axis(AxisX.current_pos);
        break;
    case MOVE_HOME2:
        AxisX.mode = MOVE_HOME3;
        return;   // thoát hàm, KHÔNG set STOP (giữ đúng logic cũ)
    default:
        break;
    }

    AxisX.mode = STOP;
    AxisX.old_pos = AxisX.current_pos;
}


void Stop_motor_y(void)
{
    output_y_pull_stop();
    switch (AxisY.mode) {
    case MOVE_MANUAL:
        AxisY.current_pos =
            AxisY.old_pos + (AxisY.dir * get_counter_timer_slave_y());
        Set_HMI_Y_Axis(AxisY.current_pos);
        reset_counter_timer_slave_y();
        break;
    case MOVE_AUTO:
        AxisY.current_pos =
            AxisY.old_pos + (AxisY.dir * (get_y_target_pull() + 1));
        Set_HMI_Y_Axis(AxisY.current_pos);
        break;
    case MOVE_HOME2:
        AxisY.mode = MOVE_HOME3;
        return;
    default:
        break;
    }
    AxisY.mode = STOP;
    AxisY.old_pos = AxisY.current_pos;
}


void Stop_motor_z(void)
{
    output_z_pull_stop();
    switch (AxisZ.mode) {
    case MOVE_MANUAL:
        AxisZ.current_pos =
            AxisZ.old_pos + (AxisZ.dir * get_counter_timer_slave_z());
        Set_HMI_Z_Axis(AxisZ.current_pos);
        reset_counter_timer_slave_z();
        break;
    case MOVE_AUTO:
        AxisZ.current_pos =
            AxisZ.old_pos + (AxisZ.dir * (get_z_target_pull() + 1));
        Set_HMI_Z_Axis(AxisZ.current_pos);
        break;
    case MOVE_HOME2:
        AxisZ.mode = MOVE_HOME3;
        return;
    default:
        break;
    }
    AxisZ.mode = STOP;
    AxisZ.old_pos = AxisZ.current_pos;
}

static inline bool AllAxisStop(void)
{
    return (AxisX.mode == STOP &&
            AxisY.mode == STOP &&
            AxisZ.mode == STOP);
}

void wait_handler_stop(){
	while (!AllAxisStop())
	{
		__NOP();
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
	 HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	 HAL_TIM_Base_Stop_IT(&htim6);
	 Cylinder1_Go_Up;
	 Cylinder2_Go_Up;
	if(get_home_z() != home_z){
		if(AxisZ.mode == STOP ){
			if(AxisZ.current_pos >0){
			AxisZ.mode = MOVE_HOME1;
				}
			}
	}
	while(AxisZ.current_pos > 3000);
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
	  wait_handler_stop();
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	  HAL_TIM_Base_Start_IT(&htim6);
}


//Cylinder_and_save
void Handle_pick_handler1(void){
//	if(PickRubber(1)){
//		Control_Vacum_Indicator->bits.pick1 = 1;
//	}
	//Control_Vacum_Indicator->bits.pick1 = PickRubber(1);
}
void Handle_release_handler1(void){
//	if(ReleaseRubber(1)){
//		Control_Vacum_Indicator->bits.realse1 = 1;
//	}
	//Control_Vacum_Indicator->bits.release1 = ReleaseRubber(1);
}
void Handle_pick_handler2(void){
//	if(PickRubber(2)){
//		Control_Vacum_Indicator->bits.pick2 = 1;
//	}
	//Control_Vacum_Indicator->bits.pick1 =  PickRubber(2);
}
void Handle_release_handler2(void){
//	if(ReleaseRubber(2)){
//		Control_Vacum_Indicator->bits.realse2 = 1;
//	}
	//Control_Vacum_Indicator->bits.release2 = ReleaseRubber(2);
}
void Handle_save1(void){
//	HAL_GPIO_TogglePin(O7_GPIO_Port, O7_Pin);

	switch(__builtin_ffs(Rubber_and_tray_indicator->all)){
	case 1:
		Rubber_Mark[0].x = AxisX.current_pos;
		Rubber_Mark[0].y = AxisY.current_pos;
		Mark[0] =  Rubber_Mark[0].x;
		Mark[1] =  Rubber_Mark[0].y;
		data[0] =  Rubber_Mark[0].raw;

		Rubber_and_tray_indicator->bits.tray_rubber_p1 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);
		break;
	case 2:
		Rubber_Mark[1].x = AxisX.current_pos;
		Rubber_Mark[1].y = AxisY.current_pos;
		Mark[2] =  Rubber_Mark[1].x;
		Mark[3] =  Rubber_Mark[1].y;
		data[1] =  Rubber_Mark[1].raw;
		Rubber_and_tray_indicator->bits.tray_rubber_p2 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);
		break;
	case 3:
		Rubber_Mark[2].x = AxisX.current_pos;
		Rubber_Mark[2].y = AxisY.current_pos;
		Mark[4] =  Rubber_Mark[2].x;
		Mark[5] =  Rubber_Mark[2].y;
		data[2] =  Rubber_Mark[2].raw;
		Rubber_and_tray_indicator->bits.tray_rubber_p3 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);
		break;
	}
	Calculate_Tray_Point(Rubber_Tray, Rubber_Mark, RUBBER_ROWS, RUBBER_COLS);
}
void Handle_save2(void){
	//HAL_GPIO_TogglePin(O8_GPIO_Port, O8_Pin);
	switch(__builtin_ffs(Rubber_and_tray_indicator->all)){
	case 4:
				Tray1_Mark[0].x = AxisX.current_pos;
				Tray1_Mark[0].y = AxisY.current_pos;
				Mark[6] =  Tray1_Mark[0].x;
				Mark[7] =  Tray1_Mark[0].y;
				data[3] =  Tray1_Mark[0].raw;
				Rubber_and_tray_indicator->bits.tray1_p1 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);

		break;
	case 5:
				Tray1_Mark[1].x = AxisX.current_pos;
				Tray1_Mark[1].y = AxisY.current_pos;
				Mark[8] =  Tray1_Mark[1].x;
				Mark[9] =  Tray1_Mark[1].y;
				data[4] =  Tray1_Mark[1].raw;
				Rubber_and_tray_indicator->bits.tray1_p2 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);
		break;
	case 6:
				Tray1_Mark[2].x = AxisX.current_pos;
				Tray1_Mark[2].y = AxisY.current_pos;
				Mark[10] =  Tray1_Mark[2].x;
				Mark[11] =  Tray1_Mark[2].y;
				data[5] =  Tray1_Mark[2].raw;
				Rubber_and_tray_indicator->bits.tray1_p3 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);
		break;
	}
	Calculate_Tray_Point(Tray1, Tray1_Mark, TRAY_ROWS, TRAY_COLS);
}
void Handle_save3(void){
	// HAL_GPIO_TogglePin(O9_GPIO_Port, O9_Pin);
	switch(__builtin_ffs(Rubber_and_tray_indicator->all)){
	case 7:
				Tray2_Mark[0].x = AxisX.current_pos;
				Tray2_Mark[0].y = AxisY.current_pos;
				Mark[12] =  Tray2_Mark[0].x;
				Mark[13] =  Tray2_Mark[0].y;
				data[6] =  Tray2_Mark[0].raw;
				Rubber_and_tray_indicator->bits.tray2_p1 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);
		break;
	case 8:
				Tray2_Mark[1].x = AxisX.current_pos;
				Tray2_Mark[1].y = AxisY.current_pos;
				Mark[14] =  Tray2_Mark[1].x;
				Mark[15] =  Tray2_Mark[1].y;
				data[7] =  Tray2_Mark[1].raw;
				Rubber_and_tray_indicator->bits.tray2_p2 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);
		break;
	case 9:
				Tray2_Mark[2].x = AxisX.current_pos;
				Tray2_Mark[2].y = AxisY.current_pos;
				Mark[16] =  Tray2_Mark[2].x;
				Mark[17] =  Tray2_Mark[2].y;
				data[8] =  Tray2_Mark[2].raw;
				Rubber_and_tray_indicator->bits.tray2_p3 = Flash_Write_Data (FlashStart, (uint32_t*)data, 10);
		break;
	}
	Calculate_Tray_Point(Tray2, Tray2_Mark, TRAY_ROWS, TRAY_COLS);
}
void Handle_load(void){
//	HAL_GPIO_TogglePin(O10_GPIO_Port, O10_Pin);

	int test_builtin1 = __builtin_ffs(Rubber_and_tray_indicator->all);
		if (test_builtin1 > 0) {
		    test_builtin1 -= 1;
		    if (test_builtin1 < (int)(sizeof(Move_tray_table) / sizeof(Move_tray_table[0]))) {
		    	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
		    		Move_tray_table[test_builtin1]();
		        	Cylinder_and_save->bits.load = 0;
				}
		    }
		}
		Cylinder_and_save->bits.load = 0;
}

//Rubber_and_tray
void Handle_tray_rubber_p1(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray_rubber_p1 = 1;
	Rubber_and_tray->bits.tray_rubber_p1 = 0 ;
}
void Handle_tray_rubber_p2(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray_rubber_p2 = 1;
	Rubber_and_tray->bits.tray_rubber_p2 = 0 ;
}
void Handle_tray_rubber_p3(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray_rubber_p3 = 1;
	Rubber_and_tray->bits.tray_rubber_p3 = 0 ;
}
void Handle_tray1_p1(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray1_p1 = 1;
	Rubber_and_tray->bits.tray1_p1 = 0 ;
}
void Handle_tray1_p2(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray1_p2 = 1;
	Rubber_and_tray->bits.tray1_p2 = 0 ;
}
void Handle_tray1_p3(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray1_p3 = 1;
	Rubber_and_tray->bits.tray1_p3 = 0 ;
}
void Handle_tray2_p1(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray2_p1 = 1;
	Rubber_and_tray->bits.tray2_p1 = 0 ;
}
void Handle_tray2_p2(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray2_p2 = 1;
	Rubber_and_tray->bits.tray2_p2 = 0 ;
}
void Handle_tray2_p3(void){
	Rubber_and_tray_indicator->all &= (~0x3ff);
	Rubber_and_tray_indicator->bits.tray2_p3 = 1;
	Rubber_and_tray->bits.tray2_p3 = 0 ;
}

void Move_tray_rubber_p1(void){
		Rubber_and_tray->bits.tray_rubber_p1 = 0 ;
		move_axis(Mark[0], Mark[1], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray_rubber_p1 = 0 ;
}
void Move_tray_rubber_p2(void){
		Rubber_and_tray->bits.tray_rubber_p2 = 0 ;
		move_axis(Mark[2], Mark[3], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray_rubber_p2 = 0 ;
}
void Move_tray_rubber_p3(void){
		Rubber_and_tray->bits.tray_rubber_p3 = 0 ;
		move_axis(Mark[4], Mark[5], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray_rubber_p3 = 0 ;
}
void Move_tray1_p1(void){
		Rubber_and_tray->bits.tray1_p1 = 0 ;
		move_axis(Mark[6], Mark[7], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray1_p1 = 0 ;
}
void Move_tray1_p2(void){
		Rubber_and_tray->bits.tray1_p2 = 0 ;
		move_axis(Mark[8], Mark[9], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray1_p2 = 0 ;
}
void Move_tray1_p3(void){
		Rubber_and_tray->bits.tray1_p3 = 0 ;
		move_axis(Mark[10], Mark[11], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray1_p3 = 0 ;
}
void Move_tray2_p1(void){
		Rubber_and_tray->bits.tray2_p1 = 0 ;
		move_axis(Mark[12], Mark[13], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray2_p1 = 0 ;
}
void Move_tray2_p2(void){
		Rubber_and_tray->bits.tray2_p2 = 0 ;
		move_axis(Mark[14], Mark[15], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray2_p2 = 0 ;
}
void Move_tray2_p3(void){
		Rubber_and_tray->bits.tray2_p3 = 0 ;
		move_axis(Mark[16], Mark[17], AxisZ.current_pos);
		Rubber_and_tray_indicator->bits.tray2_p3 = 0 ;
}
