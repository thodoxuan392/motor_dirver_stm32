/*
 * app_softPWM.h
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#ifndef INC_APP_DCMOTOR_H_
#define INC_APP_DCMOTOR_H_

#include "stdint.h"
#include "stdbool.h"
#include "app_common.h"

typedef struct {
	IO_t motor1;
	IO_t motor2;
}MOTOR_DC_t;

typedef enum{
	MOTOR_DC_A = 0,
	MOTOR_DC_B,
	MOTOR_DC_C,
	MOTOR_DC_D
}MotorLine_DC_t;

bool DCMOTOR_init();

bool DCMOTOR_setSpeed(MotorLine_DC_t motorLine, uint8_t direction, uint8_t speed);

#endif /* INC_APP_DCMOTOR_H_ */
