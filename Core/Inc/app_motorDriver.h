/*
 * app_motor_driver.h
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#ifndef INC_APP_MOTORDRIVER_H_
#define INC_APP_MOTORDRIVER_H_

#include "stdint.h"
#include "stdbool.h"

enum{
	FORWARD = 0,
	BACKWARD = 1
};

typedef enum{
	MOTOR_A = 0,
	MOTOR_B,
	MOTOR_C,
	MOTOR_D
}MotorLine_t;

bool MOTORDRIVER_init();

bool MOTORDRIVER_setSpeed(MotorLine_t motor, uint8_t direction, uint8_t speedPercent);



#endif /* INC_APP_MOTORDRIVER_H_ */
