/*
 * GP2Y1014AU.h
 *
 *  Created on: 2023/2/9
 *      Author: zzy
 */

#ifndef INC_GP2Y1014AU_H_
#define INC_GP2Y1014AU_H_

#include "main.h"
#include "ds18b20.h"

#define LED_LOW HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET) // LED关
#define LED_HIGH HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)  // LED开

float GetPMValue();

#endif /* INC_GP2Y1014AU_H_ */