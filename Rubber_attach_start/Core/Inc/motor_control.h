/*
 * motor_control.h
 *
 *  Created on: Oct 17, 2025
 *      Author: TranHoang
 */

#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_

#include "modbusRTU.h"
#include "main.h"

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

#define set_x_target_pull(pull) __HAL_TIM_SET_AUTORELOAD(&htim5, pull)
#define set_y_target_pull(pull) __HAL_TIM_SET_AUTORELOAD(&htim2, pull)
#define set_z_target_pull(pull) __HAL_TIM_SET_AUTORELOAD(&htim9, pull)


#define get_x_target_pull() __HAL_TIM_GET_AUTORELOAD(&htim5)
#define get_y_target_pull() __HAL_TIM_GET_AUTORELOAD(&htim2)
#define get_z_target_pull() __HAL_TIM_GET_AUTORELOAD(&htim9)

#define set_speed_x(speed) __HAL_TIM_SET_AUTORELOAD(&htim8, speed)
#define set_pulse_x(pulse) __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, pulse)
#define set_speed_y(speed) __HAL_TIM_SET_AUTORELOAD(&htim1, speed)
#define set_pulse_y(pulse) __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse)
#define set_speed_z(speed) __HAL_TIM_SET_AUTORELOAD(&htim3, speed)
#define set_pulse_z(pulse) __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse)

#define reset_counter_timer_x() __HAL_TIM_SET_COUNTER(&htim8, 0)
#define reset_counter_timer_y() __HAL_TIM_SET_COUNTER(&htim1, 0)
#define reset_counter_timer_z() __HAL_TIM_SET_COUNTER(&htim3, 0)
#define reset_counter_timer_slave_x() __HAL_TIM_SET_COUNTER(&htim5, 0)
#define reset_counter_timer_slave_y() __HAL_TIM_SET_COUNTER(&htim2, 0)
#define reset_counter_timer_slave_z() __HAL_TIM_SET_COUNTER(&htim9, 0)

#define max_x 50000U
#define max_y 25000U
#define max_z 10000U

#define speed_x_max (40000U)
#define speed_y_max (40000U)
#define speed_z_max (5000U)
#define speed_default (60U)
#define speed_run     (17000U)
#define speed_run_z 	(5000U)

#define speed_home1 	(4000U)
#define speed_home2 	(4000U)
#define speed_home3		(1000U)

#define speed_home1_x 	(speed_home1 + 1000)
#define speed_home2_x 	(speed_home2 + 1000)
#define speed_home3_x	(speed_home3)

#define speed_home1_y 	(speed_home1 - 2000)
#define speed_home2_y 	(speed_home2)
#define speed_home3_y	(speed_home3)

#define speed_home1_z 	(speed_home1- 3000)
#define speed_home2_z 	(speed_home2 - 2000)
#define speed_home3_z	(speed_home3- 500)

#define coil_x_left 	4U
#define coil_x_right 	5U
#define coil_y_backward 6U
#define coil_y_forward 	7U
#define coil_z_up  		8U
#define coil_z_down 	9U


#define DIR_POS   1
#define DIR_NEG  -1

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
    __IO uint16_t pulse_count;
    __IO uint16_t target_pulse;
    __IO uint8_t is_moving;
    __IO uint16_t speed;
    __IO uint16_t current_pos;
    __IO uint16_t old_pos;
     uint8_t  stop_request;
    MoveMode mode;
} Axis_t;




typedef union {
    struct {

        uint8_t x_Left	: 1;
        uint8_t x_Right	: 1;
        uint8_t y_Forward		: 1;
        uint8_t y_Backward		: 1;
        uint8_t z_Up		: 1;
        uint8_t z_Down	: 1;
        uint8_t Set		: 1;
        uint8_t GoHome	: 1;
    } bits;
    uint8_t all;
} Control_motor_t;
typedef union {
    struct {
        uint8_t pick_handler1		: 1;
        uint8_t release_handler1		: 1;
        uint8_t pick_handler2		: 1;
        uint8_t release_handler2		: 1;
        uint8_t save1		: 1;
        uint8_t save2		: 1;
        uint8_t save3	: 1;
        uint8_t load	: 1;
    } bits;
    uint8_t all;
}Cylinder_and_save_t;

typedef union {
    struct {
        uint16_t x;
        uint16_t y;
    };
    uint32_t raw;
} Point2D;

typedef union {
    struct {
        uint8_t row;
        uint8_t col;
    };
    uint32_t raw;
} TrayPos;

typedef union {
    struct {
        uint8_t tray_rubber_p1		: 1;
        uint8_t tray_rubber_p2		: 1;
        uint8_t tray_rubber_p3		: 1;
        uint8_t tray1_p1			: 1;
        uint8_t tray1_p2			: 1;
        uint8_t tray1_p3			: 1;
        uint8_t tray2_p1			: 1;
        uint8_t tray2_p2			: 1;
        uint8_t tray2_p3			: 1;
    } bits;
    uint16_t all;
}Rubber_and_tray_t;

typedef union {
    struct {
        uint8_t tray_rubber_p1		: 1;
        uint8_t tray_rubber_p2		: 1;
        uint8_t tray_rubber_p3		: 1;
        uint8_t tray1_p1			: 1;
        uint8_t tray1_p2			: 1;
        uint8_t tray1_p3			: 1;
        uint8_t tray2_p1			: 1;
        uint8_t tray2_p2			: 1;
        uint8_t tray2_p3			: 1;
        uint8_t load				:1;
    } bits;
    uint16_t all;
}Rubber_and_tray_indicator_t;

typedef union {
    struct {
    	uint8_t HOME                : 1;
    	uint8_t MOTOR               : 1;
    	uint8_t SETTING             : 1;
    	uint8_t RESERVER1           : 1;
    	uint8_t RESERVER2           : 1;
        uint8_t RESET				: 1;
        uint8_t START				: 1;
        uint8_t STOP				: 1;
    } bits;
    uint16_t all;
}ScreenMain_t;


extern uint8_t Coils_Database[25];
extern uint16_t Holding_Registers_Database[300];
extern uint8_t Inputs_Database[25];
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim9;
extern UART_HandleTypeDef huart2;



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


void Handle_X_Left (void);
void Handle_X_Right (void);
void Handle_Y_Forward(void);
void Handle_Y_Backward(void);
void Handle_Z_Up(void);
void Handle_Z_Down(void);
void Handle_Set(void);
void Handle_Home(void);
void Handle_pick_handler1(void);
void Handle_release_handler1(void);
void Handle_pick_handler2(void);
void Handle_release_handler2(void);
void Handle_save1(void);
void Handle_save2(void);
void Handle_save3(void);
void Handle_load(void);
void Handle_tray_rubber_p1(void);
void Handle_tray_rubber_p2(void);
void Handle_tray_rubber_p3(void);
void Handle_tray1_p1(void);
void Handle_tray1_p2(void);
void Handle_tray1_p3(void);
void Handle_tray2_p1(void);
void Handle_tray2_p2(void);
void Handle_tray2_p3(void);



void Control_motor_x(void);
void Control_motor_y(void);
void Control_motor_z(void);

void Stop_motor_x(void);
void Stop_motor_y(void);
void Stop_motor_z(void);

void move_axis(uint16_t xd, uint16_t yd, uint16_t zd);
void Set_HMI_X_Axis(uint16_t value);
void Set_HMI_Y_Axis(uint16_t value);
void Set_HMI_Z_Axis(uint16_t value);
uint16_t Get_HMI_X_Axis(void);
uint16_t Get_HMI_Y_Axis(void);
uint16_t Get_HMI_Z_Axis(void);
uint8_t get_coil(uint16_t reg_addr);
void update_axis_x(void);
void update_axis_y(void);
#endif /* INC_MOTOR_CONTROL_H_ */
