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

#define ON_BUZZ 		HAL_GPIO_WritePin(O12_GPIO_Port, O12_Pin, 0)
#define OFF_BUZZ		HAL_GPIO_WritePin(O12_GPIO_Port, O12_Pin, 1)

#define ON_LED_RED 		HAL_GPIO_WritePin(O11_GPIO_Port, O11_Pin, 1)
#define OFF_LED_RED 	HAL_GPIO_WritePin(O11_GPIO_Port, O11_Pin, 0)

//#define Is_Vacum1_Pick		HAL_GPIO_ReadPin(I1_GPIO_Port, I2_Pin)
//#define Is_Vacum2_Pick		HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin)

#define Cylinder1_Go_Down 	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, 0)
#define Cylinder1_Go_Up 	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, 1)
#define Cylinder2_Go_Down 	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, 0)
#define Cylinder2_Go_Up 	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, 1)

#define Vacum1_Pick			HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, 1)
							//HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, 1)
#define Vacum1_Release		HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, 1)
							//HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, 1)
#define Vacum2_Pick			HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, 1)
							//HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, 1)
#define Vacum2_Release		HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, 1)
							//HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, 1)
#define Is_Vacum1_Pick		HAL_GPIO_ReadPin(i4_vacum1_GPIO_Port, i4_vacum1_Pin)
#define Is_Vacum2_Pick		HAL_GPIO_ReadPin(i5_vacum2_GPIO_Port, i5_vacum2_Pin)

void SetBlinks(uint8_t numBlinks);
void SetBips(uint8_t numBips);
void BipControl(void);
uint8_t PickRubber(uint8_t vacum_id);
uint8_t ReleaseRubber(uint8_t vacum_id);
#endif /* INC_IO_CONTROLLER_H_ */
