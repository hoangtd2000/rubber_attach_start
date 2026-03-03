/*
 * motor_control.c
 *
 *  Created on: Oct 17, 2025
 *      Author: TranHoang
 */

#include <dio_control.h>
#include "motor_control.h"

#define truc_z_len_het
#define truc_z_len_sau


extern Control_motor_t* Control_motor;

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
