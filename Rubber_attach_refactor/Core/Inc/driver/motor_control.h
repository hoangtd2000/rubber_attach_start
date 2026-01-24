/*
 * motor_control.h
 *
 *  Created on: Oct 17, 2025
 *      Author: TranHoang
 */

#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_

#include "modbus_rtu.h"
#include "flash.h"
#include "stdbool.h"
#include "cmsis_gcc.h"

#define output_x_sig_tog()			HAL_GPIO_TogglePin(output_x_sig_GPIO_Port, output_x_sig_Pin)
#define output_y_sig_tog()			HAL_GPIO_TogglePin(output_y_sig_GPIO_Port, output_y_sig_Pin)
#define output_z_sig_tog()			HAL_GPIO_TogglePin(output_z_sig_GPIO_Port, output_z_sig_Pin)
#define output_x_sig_left() 		HAL_GPIO_WritePin(output_x_sig_GPIO_Port, output_x_sig_Pin,GPIO_PIN_SET)
#define output_x_sig_right()  		HAL_GPIO_WritePin(output_x_sig_GPIO_Port, output_x_sig_Pin,GPIO_PIN_RESET)
#define output_y_sig_forward()  	HAL_GPIO_WritePin(output_y_sig_GPIO_Port, output_y_sig_Pin,GPIO_PIN_RESET)
#define output_y_sig_backward() 	HAL_GPIO_WritePin(output_y_sig_GPIO_Port, output_y_sig_Pin,GPIO_PIN_SET)
#define output_z_sig_up()  			HAL_GPIO_WritePin(output_z_sig_GPIO_Port, output_z_sig_Pin,GPIO_PIN_SET)
#define output_z_sig_down()  		HAL_GPIO_WritePin(output_z_sig_GPIO_Port, output_z_sig_Pin,GPIO_PIN_RESET)
#define get_output_x_sig()			HAL_GPIO_ReadPin(output_x_sig_GPIO_Port, output_x_sig_Pin)
#define get_output_y_sig()			HAL_GPIO_ReadPin(output_y_sig_GPIO_Port, output_y_sig_Pin)
#define get_output_z_sig()			HAL_GPIO_ReadPin(output_z_sig_GPIO_Port, output_z_sig_Pin)
#define get_home_x()				HAL_GPIO_ReadPin(i1_home_x_GPIO_Port, i1_home_x_Pin)
#define get_home_y()				HAL_GPIO_ReadPin(i2_home_y_GPIO_Port, i2_home_y_Pin)
#define get_home_z()				HAL_GPIO_ReadPin(i3_home_z_GPIO_Port, i3_home_z_Pin)

#define home_x 1U
#define home_y 1U
#define home_z 1U

#define output_x_pull_start()	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1)
#define output_x_pull_stop()	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1)
#define output_y_pull_start()	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1)
#define output_y_pull_stop()	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1)
#define output_z_pull_start()	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3)
#define output_z_pull_stop()	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3)


#define set_counter_timer_slave_x(count)   __HAL_TIM_SET_COUNTER(&htim5, count)
#define set_counter_timer_slave_y(count)   __HAL_TIM_SET_COUNTER(&htim2, count)
#define set_counter_timer_slave_z(count)   __HAL_TIM_SET_COUNTER(&htim9, count)

#define get_counter_timer_slave_x()   __HAL_TIM_GET_COUNTER(&htim5)
#define get_counter_timer_slave_y()   __HAL_TIM_GET_COUNTER(&htim2)
#define get_counter_timer_slave_z()   __HAL_TIM_GET_COUNTER(&htim9)

#define get_counter_timer_x()   __HAL_TIM_GET_COUNTER(&htim8)
#define get_counter_timer_y()   __HAL_TIM_GET_COUNTER(&htim1)
#define get_counter_timer_z()   __HAL_TIM_GET_COUNTER(&htim3)

#define set_x_target_pull(pull) __HAL_TIM_SET_AUTORELOAD(&htim5, pull)
#define set_y_target_pull(pull) __HAL_TIM_SET_AUTORELOAD(&htim2, pull)
#define set_z_target_pull(pull) __HAL_TIM_SET_AUTORELOAD(&htim9, pull)

#define get_x_target_pull() __HAL_TIM_GET_AUTORELOAD(&htim5)
#define get_y_target_pull() __HAL_TIM_GET_AUTORELOAD(&htim2)
#define get_z_target_pull() __HAL_TIM_GET_AUTORELOAD(&htim9)

#define set_speed_x(speed) __HAL_TIM_SET_AUTORELOAD(&htim8, speed)
#define set_pulse_x(pulse) __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, pulse)
#define get_pulse_x() __HAL_TIM_GET_COMPARE(&htim8, TIM_CHANNEL_1)

#define set_speed_y(speed) __HAL_TIM_SET_AUTORELOAD(&htim1, speed)
#define set_pulse_y(pulse) __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse)
#define get_pulse_y() __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1)

#define set_speed_z(speed) __HAL_TIM_SET_AUTORELOAD(&htim3, speed)
#define set_pulse_z(pulse) __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse)
#define get_pulse_z() __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_3)

#define reset_counter_timer_x() __HAL_TIM_SET_COUNTER(&htim8, 0)
#define reset_counter_timer_y() __HAL_TIM_SET_COUNTER(&htim1, 0)
#define reset_counter_timer_z() __HAL_TIM_SET_COUNTER(&htim3, 0)
#define reset_counter_timer_slave_x() __HAL_TIM_SET_COUNTER(&htim5, 0)
#define reset_counter_timer_slave_y() __HAL_TIM_SET_COUNTER(&htim2, 0)
#define reset_counter_timer_slave_z() __HAL_TIM_SET_COUNTER(&htim9, 0)

#define max_x 55000U
#define max_y 33500U
#define max_z_tray 11500U
#define max_z_rubber 13000U
#define max_z 	(max_z_rubber)

#define speed_x_max (40000U)
#define speed_y_max (40000U)
#define speed_z_max (5000U)

#define speed_default (50U)
#define speed_run     (30000U)
#define speed_run_z 	(5000U)

#define speed_home1 	(4000U)
#define speed_home2 	(4000U)
#define speed_home3		(1000U)

//#define speed_home1_x 	(speed_home1 + 1000)
#define speed_home1_x 	(speed_home1)
#define speed_home2_x 	(speed_home2 + 1000)
#define speed_home3_x	(speed_home3)

#define speed_home1_y 	(speed_home1 - 2000)
#define speed_home2_y 	(speed_home2)
#define speed_home3_y	(speed_home3)

#define speed_home1_z 	(speed_home1- 3000)
#define speed_home2_z 	(speed_home2 - 2000)
#define speed_home3_z	(speed_home3- 500)

#define DIR_POS   1
#define DIR_NEG  -1
#define pick (1U)
#define release (0U)
typedef enum {
    DIR_STOP = 0,
    DIR_LEFT,
    DIR_RIGHT,
	DIR_FORWARD,
	DIR_BACKWARD,
	DIR_UP,
	DIR_DOWN,
	DIR_NONE
} AxisDir;
typedef enum {
	STOP = 0,
    MOVE_AUTO,
    MOVE_MANUAL,
	MOVE_HOME1,
	MOVE_HOME2,
	MOVE_HOME3
} MoveMode;

typedef struct {
    __IO uint16_t x, y, z;
} Position_t;



typedef struct {
	__IO int8_t dir;
    __IO uint16_t current_pos;
    __IO uint16_t old_pos;
    MoveMode mode;
    MoveMode pre_mode;
} Axis_t;



typedef void (*ActionHandler_t)(void);


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim9;



void excute_move_x_left(void);
void excute_move_x_right(void);
void excute_move_y_forward(void);
void excute_move_y_backward(void);
void excute_move_z_up(void);
void excute_move_z_down(void);
void Set_Speed_Motor_x(uint16_t f, uint16_t f_max);
void Set_Speed_Motor_y(uint16_t f, uint16_t f_max);
void Set_Speed_Motor_z(uint16_t f, uint16_t f_max);

void move_x_left(uint16_t d);
void move_x_right(uint16_t d);
void move_y_forward(uint16_t d);
void move_y_backward(uint16_t d);
void move_z_up(uint16_t d);
void move_z_down(uint16_t d);






void Control_motor_x(void);
void Control_motor_y(void);
void Control_motor_z(void);

void Stop_motor_x(void);
void Stop_motor_y(void);
void Stop_motor_z(void);

void wait_handler_stop();
void move_axis(uint16_t xd, uint16_t yd, uint16_t zd);
void move_axis1(uint16_t xd, uint16_t yd, uint16_t zd);


#endif /* INC_MOTOR_CONTROL_H_ */
