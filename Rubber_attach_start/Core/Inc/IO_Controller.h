/*
 * IO_Controller.h
 *
 *  Created on: Jan 14, 2026
 *      Author: CongChuc
 */

#ifndef INC_IO_CONTROLLER_H_
#define INC_IO_CONTROLLER_H_

#include "main.h"
#include "application.h"

#define ON_BUZZ 			HAL_GPIO_WritePin(O12_GPIO_Port, O12_Pin, 0)
#define OFF_BUZZ			HAL_GPIO_WritePin(O12_GPIO_Port, O12_Pin, 1)
#define TOGGLE_BUZZ 		HAL_GPIO_TogglePin(O12_GPIO_Port, O12_Pin)

#define ON_LED_RED 			HAL_GPIO_WritePin(O11_GPIO_Port, O11_Pin, 0)
#define OFF_LED_RED 		HAL_GPIO_WritePin(O11_GPIO_Port, O11_Pin, 1)
#define TOGGLE_LED_RED 		HAL_GPIO_TogglePin(O11_GPIO_Port, O11_Pin)

#define ON_LED_GREEN 		HAL_GPIO_WritePin(O10_GPIO_Port, O10_Pin, 0)
#define OFF_LED_GREEN 		HAL_GPIO_WritePin(O10_GPIO_Port, O10_Pin, 1)
#define TOGGLE_LED_GREEN 	HAL_GPIO_TogglePin(O10_GPIO_Port, O10_Pin)

#define Cylinder1_Go_Down 	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, 0)
#define Cylinder1_Go_Up 	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, 1)
#define Cylinder2_Go_Down 	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, 0)
#define Cylinder2_Go_Up 	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, 1)

#define Vacum1_Pick_On		HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, 1)
#define Vacum1_Pick_Off		HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, 0)

#define Vacum1_Release_On	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, 1)
#define Vacum1_Release_Off	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, 0)

#define Vacum2_Pick_On		HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, 1)
#define Vacum2_Pick_Off		HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, 1)
#define Vacum2_Release_On	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, 1)
#define Vacum2_Release_Off	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, 1)

#define Is_Vacum1_Pick		HAL_GPIO_ReadPin(i12_vacum1_GPIO_Port, i12_vacum1_Pin)
#define Is_Vacum2_Pick		HAL_GPIO_ReadPin(i13_vacum2_GPIO_Port, i13_vacum2_Pin)

//#define DOOR_OPEN()         (!HAL_GPIO_ReadPin(I14_Door_L_GPIO_Port, I14_Door_L_Pin) || !HAL_GPIO_ReadPin(I15_Door_R_GPIO_Port, I15_Door_R_Pin))
#define DOOR_OPEN()         (!HAL_GPIO_ReadPin(i14_Door_L_GPIO_Port, i14_Door_L_Pin))

enum {
	LED_RED,
	LED_GREEN,
};

typedef enum {
    OK = 0,
	NG,
	RUNNING,
} Result_t;

typedef enum {
    IDLE = 0,
    CYLINDER_GO_DOWN,
    CYLINDER_GO_UP,
    VACUM_REALSE_OFF,
	VACUM_REALSE_ON,
	VACUM_PICK_OFF,
    VACUM_PICK_ON,
    RETRY,
    DONE_OK,
    DONE_NG
} Cylinder_Vacum_State_t;

typedef struct {
    Result_t result;         // 0: running, 1: OK, 2: NG
    uint8_t vacum_id;
    uint8_t retry;
    uint8_t state;
} Cylinder_Vacum_Init_t;

extern Cylinder_Vacum_Init_t Handle_Pick;
extern Cylinder_Vacum_Init_t Handle_Release;

void SetBips(uint8_t numBips);
void BipControl(void);
uint8_t PickRubber(uint8_t vacum_id);
uint8_t ReleaseRubber(uint8_t vacum_id);
#endif /* INC_IO_CONTROLLER_H_ */
