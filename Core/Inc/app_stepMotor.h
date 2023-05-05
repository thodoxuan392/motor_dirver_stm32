/*
 * app_stepMotor.h
 *
 *  Created on: May 4, 2023
 *      Author: xuanthodo
 */

#ifndef INC_APP_STEPMOTOR_H_
#define INC_APP_STEPMOTOR_H_

#include "stdint.h"
#include "stdbool.h"
#include "app_common.h"

#define NUMBER_OF_STEP		200

typedef struct {
	IO_t motor1;
	IO_t motor2;
	IO_t motor3;
	IO_t motor4;
}MOTOR_Stepper_t;

typedef enum {
	MOTOR_STEP_1 = 0,
	MOTOR_STEP_2,
	MOTOR_STEP_MAX
}MotorLine_Step_t;

bool STEPMOTOR_init();

bool STEPMOTOR_setSpeed(MotorLine_Step_t motorLine, uint8_t direction, uint8_t speed);


#endif /* INC_APP_STEPMOTOR_H_ */
