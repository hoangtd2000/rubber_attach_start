/*
 * hmi_control.h
 *
 *  Created on: Jan 24, 2026
 *      Author: Admin
 */

#ifndef INC_DRIVER_HMI_CONTROL_H_
#define INC_DRIVER_HMI_CONTROL_H_

#include "dio_control.h"
#include "modbus_rtu.h"
#include "main.h"
#include "flash.h"
#include "stdbool.h"
#include "cmsis_gcc.h"
#include "motor_control.h"


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
        uint8_t pick_handler1			: 1;
        uint8_t release_handler1		: 1;
        uint8_t pick_handler2			: 1;
        uint8_t release_handler2		: 1;
        uint8_t save1					: 1;
        uint8_t save2					: 1;
        uint8_t save3					: 1;
        uint8_t load					: 1;
    } bits;
    uint8_t all;
}Cylinder_and_save_t;



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
        uint8_t load				: 1;
        uint8_t pick1				: 1;
		uint8_t release1			: 1;
		uint8_t pick2				: 1;
		uint8_t release2			: 1;
    } bits;
    uint16_t all;
}Rubber_and_tray_indicator_t;



typedef union {
    struct {
    	uint8_t home                : 1;
    	uint8_t motor               : 1;
    	uint8_t setting             : 1;
    	uint8_t RESERVER1           : 2;
        uint8_t RESET				: 1;
        uint8_t START				: 1;
        uint8_t STOP				: 1;
    } bits;
    uint8_t all;
}Taskbar_t;

typedef union {
    struct {
        uint8_t reset				: 1;
        uint8_t start				: 1;
        uint8_t stop				: 1;
    } bits;
    uint8_t all;
}Tab_main_t;

typedef union {
    struct {
        uint8_t stop				: 1;
        uint8_t next				: 1;
    } bits;
    uint8_t all;
}Tab_popup_t;

typedef union {
    struct {
    	uint8_t reserver            : 2;
        uint8_t pick1				: 1;
        uint8_t release1			: 1;
        uint8_t pick2				: 1;
        uint8_t release2			: 1;
    } bits;
    uint8_t all;
} Control_Vacum_Indicator_t;


typedef union {
    struct {
    	uint8_t err            		: 1;
        uint8_t door				: 1;
        uint8_t home 				: 1;
        uint8_t reserver			: 5;
    } bits;
    uint8_t all;
} Popup_Indicator_t;


typedef struct
{
    uint8_t is_homing : 1;   // Đang về HOME
    uint8_t is_stop   : 1;   // Dừng hệ thống
    uint8_t is_start  : 1;   // Bắt đầu chạy
    uint8_t is_err	  : 1; 	 // Gắp sịt
    uint8_t reserved  : 4;   // Dự phòng (bắt buộc để đủ 1 byte)
} SystemFlag_t;

#define Mark_all_rubber()  		(SetBit(&Inputs_Database[3], 0, 200))
#define Mark_rubber(f) 			(SetBit(&Inputs_Database[3], f, 1))
#define Clear_mark_rubber(f) 	(ClearBit(&Inputs_Database[3], f, 1))

#define Clear_all_tray1() 	(ClearBit(&Inputs_Database[28], 0, 24))
#define Mark_tray1(f) 			(SetBit(&Inputs_Database[28], f, 1))
#define Clear_mark_tray1(f) 	(ClearBit(&Inputs_Database[28], f, 1))

#define Clear_all_tray2() 	(ClearBit(&Inputs_Database[31], 0, 24))
#define Mark_tray2(f) 			(SetBit(&Inputs_Database[31], f, 1))
#define Clear_mark_tray2(f) 	(ClearBit(&Inputs_Database[31], f, 1))

#define Mark_rubber_working(f)  Input_Registers_Database[0] =  f+1
#define Mark_tray1_working(f)  Input_Registers_Database[1] =  f+1
#define Mark_tray2_working(f)  Input_Registers_Database[2] =  f+1

#define Close_Popup(f) 		(ClearBit(&Inputs_Database[34], f, 1))
#define Open_Popup(f) 			(SetBit(&Inputs_Database[34], f, 1))
#define Read_Warring(f)        (ReadBit(&Inputs_Database[34], f, 1))

#define popup_err  0
#define popup_door 1
#define popup_home 2

#define Clear_Rubber_and_tray_indicator() Rubber_and_tray_indicator->all &= (~0x1ff)

void Set_HMI_X_Axis(uint16_t value);
void Set_HMI_Y_Axis(uint16_t value);
void Set_HMI_Z_Axis(uint16_t value);
uint16_t Get_HMI_X_Axis(void);
uint16_t Get_HMI_Y_Axis(void);
uint16_t Get_HMI_Z_Axis(void);

//taskbar
void Handle_main(void);
void Handle_motor(void);
void Handle_setting(void);
//tab main
void Handle_reset(void);
void Handle_start(void);
void Handle_stop(void);
//tab_motor
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
void Move_tray_rubber_p1(void);
void Move_tray_rubber_p2(void);
void Move_tray_rubber_p3(void);
void Move_tray1_p1(void);
void Move_tray1_p2(void);
void Move_tray1_p3(void);
void Move_tray2_p1(void);
void Move_tray2_p2(void);
void Move_tray2_p3(void);

#endif /* INC_DRIVER_HMI_CONTROL_H_ */
