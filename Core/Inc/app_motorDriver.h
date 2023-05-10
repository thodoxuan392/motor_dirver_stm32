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

#define NUMBER_OF_STEP



typedef enum {
	DC_MOTOR,
	STEP_MOTOR
}MotorType_t;

bool MOTORDRIVER_init();


#endif /* INC_APP_MOTORDRIVER_H_ */
