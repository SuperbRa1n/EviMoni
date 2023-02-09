/*
 * HC-SR04.h
 *
 *  Created on: 2023/2/9
 *      Author: zzy
 */

#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_

#include "main.h"
#include "ds18b20.h"
#include "tim.h"

void Start(void);
int Get(void);
void start(void);
unsigned int get_stop(void);

#endif /* INC_HCSR04_H_ */