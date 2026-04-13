/*
 * hmi_control.c
 *
 *  Created on: Jan 24, 2026
 *      Author: Admin
 */


#include "hmi_control.h"

Taskbar_t* Taskbar = (Taskbar_t*)&Coils_Database[0];
Tab_main_t* Tab_main = (Tab_main_t*)&Coils_Database[1];
Control_motor_t* Control_motor = (Control_motor_t*)&Coils_Database[2];
Savepoint_and_picker_t* Savepoint_and_picker = (Savepoint_and_picker_t*)&Coils_Database[3];
Tab_setting_t* Tab_setting =  (Tab_setting_t*)&Coils_Database[8];
Choose_model_t* Choose_model = (Choose_model_t*)&Coils_Database[9];
Rubber_and_tray_indicator_t* Rubber_and_tray_indicator = (Rubber_and_tray_indicator_t*)&Inputs_Database[1];

Tab_popup_t* Tab_popup = (Tab_popup_t*)&Coils_Database[6];

Tab_main_t* Tab_main_indicator = (Tab_main_t*) &Inputs_Database[0];

Savepoint_and_picker_indicator_t* Savepoint_and_picker_indicator = (Savepoint_and_picker_indicator_t*)&Inputs_Database[1];

State_picker_t* State_picker = (State_picker_t*)&Holding_Registers_Database[33];
xilanh_and_vacum_indicator_t* xilanh_and_vacum_indicator = (xilanh_and_vacum_indicator_t*)&Inputs_Database[35];
Popup_Indicator_t* Popup_Indicator = (Popup_Indicator_t*)&Inputs_Database[34];
Point3D Rubber_Mark[3];
Point3D Tray1_Mark[3];
Point3D Tray2_Mark[3];
uint16_t* Mark = &Holding_Registers_Database[6];
uint64_t data[10];
const uint32_t FlashStart = 0x08010000;

extern Axis_t AxisX, AxisY, AxisZ;
extern Point3D Rubber_Tray[200];
extern Point3D Tray1[30];
extern Point3D Tray2[30];

volatile SystemFlag_t SystemFlag={
		.is_homing = 0 ,
		.is_start = 0,
		.is_stop= 0,
		.is_err = 0,
};


ActionHandler_t Tab_main_table[] =  {
		 Handle_set,
		 Handle_start,
		 Handle_stop,
};


ActionHandler_t Tab_motor_table[] =  {
		 Handle_X_Left,
		 Handle_X_Right,
		 Handle_Y_Backward,
		 Handle_Y_Forward,
		 Handle_Z_Up,
		 Handle_Z_Down,
		 Handle_Origin,
		 Handle_Home,
};
ActionHandler_t Save_and_picker_table[] =  {
	Handle_save_trayrubber,
	Handle_save_tray1,
	Handle_save_tray2,
	Handle_move,
	Handle_tray_rubber_p1,
	Handle_tray_rubber_p2,
	Handle_tray_rubber_p3,
	Handle_tray1_p1,
	Handle_tray1_p2,
	Handle_tray1_p3,
	Handle_tray2_p1,
	Handle_tray2_p2,
	Handle_tray2_p3,
	Handle_xilanh1,
	Handle_xilanh2,
	Handle_xilanh3,
	Handle_xilanh4,
	Handle_xilanh5,
	Handle_xilanh6,
	Handle_vacum1,
	Handle_vacum2,
	Handle_vacum3,
	Handle_vacum4,
	Handle_vacum5,
	Handle_vacum6
};


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

// taskbar
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
	uint8_t builtin_Handle_motor = __builtin_ffs( Control_motor->all);
		if (builtin_Handle_motor > 0) {
			builtin_Handle_motor -= 1;
		    if (builtin_Handle_motor < (int)(sizeof(Tab_motor_table) / sizeof(Tab_motor_table[0]))) {
		    	Tab_motor_table[builtin_Handle_motor]();
		    }
		}

		uint8_t builtin_Handle_save_and_picker = __builtin_ffs(Savepoint_and_picker->all);
			if (builtin_Handle_save_and_picker > 0) {
				builtin_Handle_save_and_picker -= 1;
			    if (builtin_Handle_save_and_picker < (int)(sizeof(Save_and_picker_table) / sizeof(Save_and_picker_table[0]))) {
			    	Save_and_picker_table[builtin_Handle_save_and_picker]();
			    }
			}
}
void Handle_setting(void){

}

extern volatile uint16_t tray_index;   // 0..199
void Handle_set(void){
	Tab_main->bits.set = 0;
	tray_index = Holding_Registers_Database[3] - 1;
	if(tray_index >= 0 && tray_index <= 200){
		Set_item_rubber(tray_index);
		Clear_item_rubber(tray_index);
		Holding_Registers_Database[3] = 0;
	}
}
void Handle_start(void){
	if(SystemFlag.is_homing || Tab_main_indicator->bits.start ){
		Tab_main->bits.start = 0;
		return ;
	}
	SystemFlag.is_err = 0 ;
	Tab_main_indicator->bits.start =  1 ;
	Handle();
}
void Handle_stop(void){
	if(!SystemFlag.is_err  && !Tab_main_indicator->bits.stop && Tab_main_indicator->bits.start){
		Tab_main_indicator->bits.stop =  1 ;
		SystemFlag.is_stop = 1 ;
	}
}


// tab motor
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

void Handle_Origin(void){
	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
	move_axis(Get_HMI_X_Axis(),Get_HMI_Y_Axis(), Get_HMI_Z_Axis());
	}
}
void Handle_Home(void){
	SystemFlag.is_homing = 1 ;
		Open_Popup(popup_home);

	  if(get_home_z() == home_z){
		  AxisZ.mode = MOVE_HOME2;
	  }else {
		  AxisZ.mode = MOVE_HOME1;
	  }
	  Home_process_z();
		while((AxisZ.mode != MOVE_HOME3));
		while((AxisZ.mode != STOP));
	  if(get_home_x() == home_x){
		  AxisX.mode = MOVE_HOME2;
	  }else{
		  AxisX.mode = MOVE_HOME1;
	  }
	  Home_process_x();
	  if(get_home_y() == home_y){
		  AxisY.mode = MOVE_HOME2;
	  }else{
		  AxisY.mode = MOVE_HOME1;
	  }
	  Home_process_y();
	  wait_handler_stop();
	  SystemFlag.is_homing = 0 ;
	  Close_Popup(popup_home);
}

/**
 * @brief Nhóm hàm lưu các điểm chuẩn Rubber, Tray1, Tray2 (P1, P2, P3)
 *
 * Chức năng:
 * - Dựa vào bit đang active trong Rubber_and_tray_indicator->all
 * - Lấy vị trí hiện tại của trục X/Y
 * - Lưu vào mảng Rubber_Mark tương ứng
 * - Ghi giá trị X/Y vào Holding Register (Mark[])
 * - Ghi dữ liệu raw xuống Flash
 * - Sau khi đủ điểm, tính toán lại tọa độ điểm
 *
 * Điều kiện:
 * - Chỉ xử lý 1 bit active tại một thời điểm
 * - Mapping Holding Register bắt đầu từ Mark[0]
 */

static uint8_t SaveMark(Point3D *markArray,
                     uint8_t markIndex,
                     uint16_t markOffset,
                     uint8_t dataIndex)
{
    markArray[markIndex].x = AxisX.current_pos;
    markArray[markIndex].y = AxisY.current_pos;
    markArray[markIndex].z = AxisZ.current_pos;

    Mark[markOffset + 0] = markArray[markIndex].x;
    Mark[markOffset + 1] = markArray[markIndex].y;
    Mark[markOffset + 2] = markArray[markIndex].z;

    data[dataIndex] = markArray[markIndex].raw;

    return Flash_Write_Data(FlashStart, (uint64_t*)data, 10);
}

void Handle_save_trayrubber(void){
	Savepoint_and_picker->bits.save_trayrubber = 0 ;
	 switch(__builtin_ffs(Rubber_and_tray_indicator->all))
	    {
	        case 1:
	            Rubber_and_tray_indicator->bits.tray_rubber_p1 = SaveMark(Rubber_Mark, 0, 0, 0);
	            break;
	        case 2:
	            Rubber_and_tray_indicator->bits.tray_rubber_p2 = SaveMark(Rubber_Mark, 1, 3, 1);
	            break;
	        case 3:
	            Rubber_and_tray_indicator->bits.tray_rubber_p3 =  SaveMark(Rubber_Mark, 2, 6, 2);
	            break;
	        default:
	            return;
	    }

	    Calculate_TrayRubber_Point(Rubber_Tray, Rubber_Mark,
	                               RUBBER_ROWS, RUBBER_COLS);
}
void Handle_save_tray1(void){
	Savepoint_and_picker->bits.save_tray1 = 0;
	 switch(__builtin_ffs(Rubber_and_tray_indicator->all))
	    {
	        case 4:
	            Rubber_and_tray_indicator->bits.tray1_p1 =  SaveMark(Tray1_Mark, 0, 9, 3);
	            break;
	        case 5:
	            Rubber_and_tray_indicator->bits.tray1_p2 = SaveMark(Tray1_Mark, 1, 12, 4);
	            break;
	        case 6:
	            Rubber_and_tray_indicator->bits.tray1_p3 = SaveMark(Tray1_Mark, 2, 15, 5);
	            break;
	        default:
	            return;
	    }

	Calculate_Tray_Point(Tray1, Tray1_Mark, TRAY_ROWS, TRAY_COLS);
}
void Handle_save_tray2(void){
	Savepoint_and_picker->bits.save_tray2 = 0;
	switch(__builtin_ffs(Rubber_and_tray_indicator->all))
	    {
	        case 7:
	            Rubber_and_tray_indicator->bits.tray2_p1 =  SaveMark(Tray2_Mark, 0, 18, 6);
	            break;
	        case 8:
	            Rubber_and_tray_indicator->bits.tray2_p2 =  SaveMark(Tray2_Mark, 1, 21, 7);
	            break;
	        case 9:
	            Rubber_and_tray_indicator->bits.tray2_p3 =  SaveMark(Tray2_Mark, 2, 24, 8);
	            break;
	        default:
	            return;
	    }

	Calculate_Tray_Point(Tray2, Tray2_Mark, TRAY_ROWS, TRAY_COLS);
}
/**
 * @brief Nhóm hàm chọn điểm chuẩn Tray/Rubber từ HMI
 *
 * Chức năng chung:
 * - Xóa toàn bộ trạng thái trong Rubber_and_tray_indicator
 * - Kích hoạt duy nhất 1 bit indicator tương ứng với điểm được chọn (P1 / P2 / P3)
 * - Reset bit lệnh từ HMI (Rubber_and_tray) sau khi xử lý
 *
 * Mục đích:
 * - Đảm bảo chỉ có một điểm chuẩn được chọn tại một thời điểm
 * - Làm điều kiện cho các thao tác Save / Load / Move tiếp theo
 *
 * Lưu ý:
 * - Các hàm trong nhóm này có cùng logic, chỉ khác bit indicator được set
 * - Không xử lý chuyển động trục trong các hàm này
 */
void Handle_tray_rubber_p1(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray_rubber_p1 = 1;
	Savepoint_and_picker->bits.tray_rubber_p1 = 0 ;
}
void Handle_tray_rubber_p2(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray_rubber_p2 = 1;
	Savepoint_and_picker->bits.tray_rubber_p2 = 0 ;
}
void Handle_tray_rubber_p3(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray_rubber_p3 = 1;
	Savepoint_and_picker->bits.tray_rubber_p3 = 0 ;
}
void Handle_tray1_p1(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray1_p1 = 1;
	Savepoint_and_picker->bits.tray1_p1 = 0 ;
}
void Handle_tray1_p2(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray1_p2 = 1;
	Savepoint_and_picker->bits.tray1_p2 = 0 ;
}
void Handle_tray1_p3(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray1_p3 = 1;
	Savepoint_and_picker->bits.tray1_p3 = 0 ;
}
void Handle_tray2_p1(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray2_p1 = 1;
	Savepoint_and_picker->bits.tray2_p1 = 0 ;
}
void Handle_tray2_p2(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray2_p2 = 1;
	Savepoint_and_picker->bits.tray2_p2 = 0 ;
}
void Handle_tray2_p3(void){
	Clear_Rubber_and_tray_indicator();
	Rubber_and_tray_indicator->bits.tray2_p3 = 1;
	Savepoint_and_picker->bits.tray2_p3 = 0 ;
}



/**
 * @brief Nhóm hàm di chuyển trục tới các điểm Tray/Rubber đã lưu
 *
 * Chức năng chung:
 * - Xóa bit lệnh di chuyển tương ứng từ HMI (Rubber_and_tray)
 * - Đọc tọa độ X/Y đã lưu trong Holding Register (Mark[])
 * - Gọi hàm move_axis() để di chuyển các trục tới vị trí đã chọn
 * - Giữ nguyên vị trí trục Z hiện tại
 *
 * Mapping:
 * - Mỗi hàm tương ứng với 1 điểm (P1 / P2 / P3) của Rubber, Tray1 hoặc Tray2
 * - Tọa độ được lấy theo cặp Mark[index], Mark[index+1]
 *
 * Lưu ý:
 * - Các hàm trong nhóm này có cùng logic, chỉ khác offset của Mark[]
 * - Không kiểm tra trạng thái trục (được kiểm soát ở tầng gọi)
 */
void Move_tray_rubber_p1(void){
		//Rubber_and_tray->bits.tray_rubber_p1 = 0 ;
		move_axis(Rubber_Mark[0].x, Rubber_Mark[0].y, Rubber_Mark[0].z);
}
void Move_tray_rubber_p2(void){
	//	Rubber_and_tray->bits.tray_rubber_p2 = 0 ;
		move_axis(Rubber_Mark[1].x,Rubber_Mark[1].y, Rubber_Mark[1].z);
}
void Move_tray_rubber_p3(void){
		//Rubber_and_tray->bits.tray_rubber_p3 = 0 ;
		move_axis(Rubber_Mark[2].x, Rubber_Mark[2].y, Rubber_Mark[2].z);
}
void Move_tray1_p1(void){
	//	Rubber_and_tray->bits.tray1_p1 = 0 ;
		move_axis(Tray1_Mark[0].x,Tray1_Mark[0].y,Tray1_Mark[0].z);
}
void Move_tray1_p2(void){
		//Rubber_and_tray->bits.tray1_p2 = 0 ;
		move_axis(Tray1_Mark[1].x, Tray1_Mark[1].y, Tray1_Mark[1].z);
}
void Move_tray1_p3(void){
		//Rubber_and_tray->bits.tray1_p3 = 0 ;
		move_axis(Tray1_Mark[2].x, Tray1_Mark[2].y, Tray1_Mark[2].z);
}
void Move_tray2_p1(void){
		//Rubber_and_tray->bits.tray2_p1 = 0 ;
		move_axis(Tray2_Mark[0].x, Tray2_Mark[0].y, Tray2_Mark[0].z);
}
void Move_tray2_p2(void){
		//Rubber_and_tray->bits.tray2_p2 = 0 ;
		move_axis(Tray2_Mark[1].x, Tray2_Mark[1].y, Tray2_Mark[1].z);
}
void Move_tray2_p3(void){
		//Rubber_and_tray->bits.tray2_p3 = 0 ;
		move_axis(Tray2_Mark[2].x, Tray2_Mark[2].y, Tray2_Mark[2].z);
}

/**
 * @brief Thực hiện thao tác LOAD theo vị trí Tray/Rubber được chọn
 *
 * Chức năng:
 * - Đọc bit chỉ thị đang active trong Rubber_and_tray_indicator->all
 * - Xác định index tương ứng trong bảng hàm Move_tray_table
 * - Chỉ thực hiện khi tất cả các trục (X, Y, Z) đang ở trạng thái STOP
 * - Gọi hàm di chuyển tương ứng để đưa cơ cấu tới vị trí đã lưu
 * - Chờ quá trình di chuyển hoàn tất
 * - Xóa bit chỉ thị sau khi load xong
 * - Reset cờ LOAD từ HMI/Cylinder
 *
 * Điều kiện an toàn:
 * - Chỉ cho phép 1 bit indicator được bật tại một thời điểm
 * - Không thực hiện load khi bất kỳ trục nào đang chuyển động
 *
 * Lưu ý:
 * - __builtin_ffs() trả về vị trí bit đầu tiên = 1 (bắt đầu từ 1)
 * - test_builtin1 được giảm 1 để map đúng index mảng Move_tray_table[]
 */
void Handle_move(void){
	int test_builtin1 = __builtin_ffs(Rubber_and_tray_indicator->all);

		if (test_builtin1 > 0) {
		    test_builtin1 -= 1;
		    if (test_builtin1 < (int)(sizeof(Move_tray_table) / sizeof(Move_tray_table[0]))) {
		    	if(AxisX.mode == STOP && AxisY.mode == STOP && AxisZ.mode == STOP ){
		    		Reset_xilanh();
		    		delay_us(200);
		    		//wait_handler_stop();
		    		Move_tray_table[test_builtin1]();
		    		wait_handler_stop();
		    		Clear_Rubber_and_tray_indicator();
		    		Rubber_and_tray_indicator->bits.move = 0 ;
				}
		    }
		}
		Savepoint_and_picker->bits.move = 0;
}





//void Handle_picker1(void){
//	Handler_picker(0,State_picker->state_picker1 );
//	//Holding_Registers_Database[33] = 0;
//	//State_picker->state_picker1 = 0 ;
//	//Savepoint_and_picker->bits.picker1 = 0 ;
//}
//void Handle_picker2(void){
//	Handler_picker(1,State_picker->state_picker2 );
//	//Holding_Registers_Database[34] = 0 ;
//	//State_picker->state_picker2 = 0 ;
//	//Savepoint_and_picker->bits.picker2 = 0 ;
//}
//void Handle_picker3(void){
//	Handler_picker(2,State_picker->state_picker3);
//	//Holding_Registers_Database[35]= 0 ;
//	//State_picker->state_picker3 = 0 ;
//	//Savepoint_and_picker->bits.picker3 = 0 ;
//}
//void Handle_picker4(void){
//	Handler_picker(3,State_picker->state_picker4);
////	Holding_Registers_Database[36] = 0 ;
//	//State_picker->state_picker4 = 0 ;
//	//Savepoint_and_picker->bits.picker4 = 0 ;
//}
//void Handle_picker5(void){
//	Handler_picker(4,State_picker->state_picker5);
////	Holding_Registers_Database[37] = 0 ;
//	//State_picker->state_picker5 = 0 ;
//	//Savepoint_and_picker->bits.picker5 = 0 ;
//}
//void Handle_picker6(void){
//	Handler_picker(5,State_picker->state_picker6);
////	Holding_Registers_Database[38] = 0 ;
////	State_picker->state_picker6 = 0 ;
//	//Savepoint_and_picker->bits.picker6 = 0 ;
//}


void Handle_xilanh1(void){
	xilanh_and_vacum_indicator->bits.xilanh1 ^= 1;
	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, !xilanh_and_vacum_indicator->bits.xilanh1);
	Savepoint_and_picker->bits.xilanh1 = 0 ;
}
void Handle_xilanh2(void){
	xilanh_and_vacum_indicator->bits.xilanh2 ^= 1;
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, !xilanh_and_vacum_indicator->bits.xilanh2);
	Savepoint_and_picker->bits.xilanh2 = 0 ;
}
void Handle_xilanh3(void){
	xilanh_and_vacum_indicator->bits.xilanh3 ^= 1;
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, !xilanh_and_vacum_indicator->bits.xilanh3);
	Savepoint_and_picker->bits.xilanh3 = 0 ;
}
void Handle_xilanh4(void){
	xilanh_and_vacum_indicator->bits.xilanh4 ^= 1;
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, !xilanh_and_vacum_indicator->bits.xilanh4);
	Savepoint_and_picker->bits.xilanh4 = 0 ;
}
void Handle_xilanh5(void){
	xilanh_and_vacum_indicator->bits.xilanh5 ^= 1;
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, !xilanh_and_vacum_indicator->bits.xilanh5);
	Savepoint_and_picker->bits.xilanh5 = 0 ;
}
void Handle_xilanh6(void){
	xilanh_and_vacum_indicator->bits.xilanh6 ^= 1;
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, !xilanh_and_vacum_indicator->bits.xilanh6);
	Savepoint_and_picker->bits.xilanh6 = 0 ;
}

void Handle_vacum1(void){
	xilanh_and_vacum_indicator->bits.vacum1 ^= 1;
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, !xilanh_and_vacum_indicator->bits.vacum1);
	Savepoint_and_picker->bits.vacum1 = 0 ;
}
void Handle_vacum2(void){
	xilanh_and_vacum_indicator->bits.vacum2 ^= 1;
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, !xilanh_and_vacum_indicator->bits.vacum2);
	Savepoint_and_picker->bits.vacum2 = 0 ;
}
void Handle_vacum3(void){
	xilanh_and_vacum_indicator->bits.vacum3 ^= 1;
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, !xilanh_and_vacum_indicator->bits.vacum3);
	Savepoint_and_picker->bits.vacum3 = 0 ;
}
void Handle_vacum4(void){
	xilanh_and_vacum_indicator->bits.vacum4 ^= 1;
	HAL_GPIO_WritePin(O10_GPIO_Port, O10_Pin, !xilanh_and_vacum_indicator->bits.vacum4);
	Savepoint_and_picker->bits.vacum4 = 0 ;
}
void Handle_vacum5(void){
	xilanh_and_vacum_indicator->bits.vacum5 ^= 1;
	HAL_GPIO_WritePin(O11_GPIO_Port, O11_Pin, !xilanh_and_vacum_indicator->bits.vacum5);
	Savepoint_and_picker->bits.vacum5 = 0 ;
}
void Handle_vacum6(void){
	xilanh_and_vacum_indicator->bits.vacum6 ^= 1;
	HAL_GPIO_WritePin(O12_GPIO_Port, O12_Pin, !xilanh_and_vacum_indicator->bits.vacum6);
	Savepoint_and_picker->bits.vacum6 = 0 ;
}
void Reset_xilanh(void){
		xilanh_and_vacum_indicator->bits.xilanh1 = 0;
		HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, !xilanh_and_vacum_indicator->bits.xilanh1);
		xilanh_and_vacum_indicator->bits.xilanh2 = 0;
		HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, !xilanh_and_vacum_indicator->bits.xilanh2);
//		xilanh_and_vacum_indicator->bits.xilanh3 = 0;
//		HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, !xilanh_and_vacum_indicator->bits.xilanh3);
//		xilanh_and_vacum_indicator->bits.xilanh4 = 0;
//		HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, !xilanh_and_vacum_indicator->bits.xilanh4);
//		xilanh_and_vacum_indicator->bits.xilanh5 = 0;
//		HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, !xilanh_and_vacum_indicator->bits.xilanh5);
//		xilanh_and_vacum_indicator->bits.xilanh6 = 0;
//		HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, !xilanh_and_vacum_indicator->bits.xilanh6);
}
