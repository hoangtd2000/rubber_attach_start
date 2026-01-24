/*
 * hmi_control.c
 *
 *  Created on: Jan 24, 2026
 *      Author: Admin
 */


#include "hmi_control.h"


Control_motor_t* Control_motor = (Control_motor_t*)&Coils_Database[1];
Cylinder_and_save_t* Cylinder_and_save = (Cylinder_and_save_t*)&Coils_Database[2];
Rubber_and_tray_t* Rubber_and_tray  = (Rubber_and_tray_t*)&Coils_Database[3];
Rubber_and_tray_indicator_t* Rubber_and_tray_indicator = (Rubber_and_tray_indicator_t*)&Inputs_Database[1];
Control_Vacum_Indicator_t* Control_Vacum_Indicator = (Control_Vacum_Indicator_t*)&Inputs_Database[2];

Point2D Rubber_Mark[3];
Point2D Tray1_Mark[3];
Point2D Tray2_Mark[3];
uint16_t* Mark = &Holding_Registers_Database[6];
uint32_t data[10];
const uint32_t FlashStart = 0x0800C000;

extern Axis_t AxisX, AxisY, AxisZ;
extern Item Rubber_Tray[400];
extern Item Tray1[30];
extern Item Tray2[30];

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

 void Set_HMI_X_Axis(uint16_t value){
	 Input_Registers_Database[11] = value;
}
 void Set_HMI_Y_Axis(uint16_t value){
	 Input_Registers_Database[12] = value ;
}
void Set_HMI_Z_Axis(uint16_t value){
	Input_Registers_Database[13] = value ;
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




void Handle_X_Left (void){
	if(AxisX.current_pos <= 0) {
		return;
		}
	if(AxisX.mode == STOP) {
		AxisX.mode =  MOVE_MANUAL;
		move_x_left(AxisX.current_pos);
		}
}
void Handle_X_Right (void){
	if(AxisX.current_pos >= max_x){
		return;
	}
	if(AxisX.mode == STOP) {
	  AxisX.mode =  MOVE_MANUAL;
	  move_x_right(max_x - AxisX.current_pos);
	}

}
void Handle_Y_Forward(void){
	if(AxisY.current_pos >= max_y) {
		return;
		}
	if(AxisY.mode == STOP){
		AxisY.mode = MOVE_MANUAL;
		move_y_forward(max_y- AxisY.current_pos);
	}
}
void Handle_Y_Backward(void){
	if(AxisY.current_pos <= 0  ){
		return ;
	}
	if(AxisY.mode == STOP){
		AxisY.mode = MOVE_MANUAL;
		move_y_backward( AxisY.current_pos);
	}
}
void Handle_Z_Up(void){
	if(AxisZ.current_pos <=0  ){
			return ;
		}
	if(AxisZ.mode == STOP) {
		AxisZ.mode = MOVE_MANUAL;
		move_z_up(AxisZ.current_pos);
	}
}
void Handle_Z_Down(void){
	if(AxisZ.current_pos >=max_z  ){
			return ;
		}
	if(AxisZ.mode == STOP) {
		AxisZ.mode = MOVE_MANUAL;
		move_z_down(max_z-AxisZ.current_pos);
	}

}

void Handle_Set(void){
	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
	move_axis(Get_HMI_X_Axis(),Get_HMI_Y_Axis(), Get_HMI_Z_Axis());
	}
}
void Handle_Home(void){
	Open_Popup(popup_home);
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
	  Close_Popup(popup_home);
}


//Cylinder_and_save
void Handle_pick_handler1(void){
	Control_Vacum_Indicator->bits.pick1 = PickRubber(1);
}
void Handle_release_handler1(void){
	Control_Vacum_Indicator->bits.release1 = ReleaseRubber(1);
}
void Handle_pick_handler2(void){
	Control_Vacum_Indicator->bits.pick1 =  PickRubber(2);
}
void Handle_release_handler2(void){
	Control_Vacum_Indicator->bits.release2 = ReleaseRubber(2);
}
void Handle_save1(void){

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
	Calculate_TrayRubber_Point(Rubber_Tray, Rubber_Mark, RUBBER_ROWS, RUBBER_COLS);
}
void Handle_save2(void){
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

//Rubber_and_tray
void Handle_tray_rubber_p1(void){
//	Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray_rubber_p1 = 1;
	Rubber_and_tray->bits.tray_rubber_p1 = 0 ;
}
void Handle_tray_rubber_p2(void){
//	Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray_rubber_p2 = 1;
	Rubber_and_tray->bits.tray_rubber_p2 = 0 ;
}
void Handle_tray_rubber_p3(void){
	//Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray_rubber_p3 = 1;
	Rubber_and_tray->bits.tray_rubber_p3 = 0 ;
}
void Handle_tray1_p1(void){
	//Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray1_p1 = 1;
	Rubber_and_tray->bits.tray1_p1 = 0 ;
}
void Handle_tray1_p2(void){
	//Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray1_p2 = 1;
	Rubber_and_tray->bits.tray1_p2 = 0 ;
}
void Handle_tray1_p3(void){
//	Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray1_p3 = 1;
	Rubber_and_tray->bits.tray1_p3 = 0 ;
}
void Handle_tray2_p1(void){
	//Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray2_p1 = 1;
	Rubber_and_tray->bits.tray2_p1 = 0 ;
}
void Handle_tray2_p2(void){
//	Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray2_p2 = 1;
	Rubber_and_tray->bits.tray2_p2 = 0 ;
}
void Handle_tray2_p3(void){
//	Rubber_and_tray_indicator->all &= (~0x1ff);
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray2_p3 = 1;
	Rubber_and_tray->bits.tray2_p3 = 0 ;
}

void Move_tray_rubber_p1(void){
		Rubber_and_tray->bits.tray_rubber_p1 = 0 ;
		move_axis(Mark[0], Mark[1], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray_rubber_p1 = 0 ;
}
void Move_tray_rubber_p2(void){
		Rubber_and_tray->bits.tray_rubber_p2 = 0 ;
		move_axis(Mark[2], Mark[3], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray_rubber_p2 = 0 ;
}
void Move_tray_rubber_p3(void){
		Rubber_and_tray->bits.tray_rubber_p3 = 0 ;
		move_axis(Mark[4], Mark[5], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray_rubber_p3 = 0 ;
}
void Move_tray1_p1(void){
		Rubber_and_tray->bits.tray1_p1 = 0 ;
		move_axis(Mark[6], Mark[7], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray1_p1 = 0 ;
}
void Move_tray1_p2(void){
		Rubber_and_tray->bits.tray1_p2 = 0 ;
		move_axis(Mark[8], Mark[9], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray1_p2 = 0 ;
}
void Move_tray1_p3(void){
		Rubber_and_tray->bits.tray1_p3 = 0 ;
		move_axis(Mark[10], Mark[11], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray1_p3 = 0 ;
}
void Move_tray2_p1(void){
		Rubber_and_tray->bits.tray2_p1 = 0 ;
		move_axis(Mark[12], Mark[13], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray2_p1 = 0 ;
}
void Move_tray2_p2(void){
		Rubber_and_tray->bits.tray2_p2 = 0 ;
		move_axis(Mark[14], Mark[15], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray2_p2 = 0 ;
}
void Move_tray2_p3(void){
		Rubber_and_tray->bits.tray2_p3 = 0 ;
		move_axis(Mark[16], Mark[17], AxisZ.current_pos);
	//	Rubber_and_tray_indicator->bits.tray2_p3 = 0 ;
}

void Handle_load(void){

	int test_builtin1 = __builtin_ffs(Rubber_and_tray_indicator->all);
		if (test_builtin1 > 0) {
		    test_builtin1 -= 1;
		    if (test_builtin1 < (int)(sizeof(Move_tray_table) / sizeof(Move_tray_table[0]))) {
		    	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
		    		Move_tray_table[test_builtin1]();
		    		wait_handler_stop();
		    		Clear_Rubber_and_tray_indicator();
		    		//Rubber_and_tray_indicator->all &= (~0x1ff);
		        	Cylinder_and_save->bits.load = 0;

				}
		    }
		}
		Cylinder_and_save->bits.load = 0;
}
