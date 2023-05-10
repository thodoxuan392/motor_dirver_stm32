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

#define SPEED_DEFAULT	100	// rpm
#define SINGLE_STEP_MAX		2
#define DOUBLE_STEP_MAX		4
#define INTERLEAVE_STEP_MAX	8
#define MICRO_STEP_MAX		16

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

typedef enum {
	SINGLE = 0,
	DOUBLE,
	INTERLEAVE,
	MICROSTEP,
	STYLE_MAX
}StepperStyle_t;


typedef enum {
	SPEED = 0,
	STEP
}StepperMode_t;

bool STEPMOTOR_init();

bool STEPMOTOR_setSpeed(MotorLine_Step_t motorLine, uint16_t number_step, uint8_t direction, uint16_t speed, StepperStyle_t style);
bool STEPMOTOR_step(MotorLine_Step_t motorLine, uint16_t number_step, uint8_t direction, uint16_t step, StepperStyle_t style);


#endif /* INC_APP_STEPMOTOR_H_ */
