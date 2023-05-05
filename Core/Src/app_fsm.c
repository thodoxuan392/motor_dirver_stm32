/*
 * app_fsm.c
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#include "stdio.h"
#include "app_fsm.h"
#include "app_i2c.h"

typedef enum{
	FSM_WAIT_I2C_REQUEST,
}FSMState_t;

static FSMState_t fsm_state = FSM_WAIT_I2C_REQUEST;

bool FSM_init(){
	// Initialize State of FSM
	fsm_state = FSM_WAIT_I2C_REQUEST;
}

bool FSM_run(){
	switch (fsm_state) {
		case FSM_WAIT_I2C_REQUEST:
			FSM_wait_i2c_request();
			break;
		default:
			break;
	}
}

void FSM_wait_i2c_request(){
	// Get I2C Request -> Static for ignore release stack because it'called many time = Ơ
	static I2CData_t i2cData;
	if(I2C_receive_data(&i2cData)){
		// Get Motor Type
		uint8_t motorType = i2cData.data[0];
		// Get MotorLine
		uint8_t motorLine = i2cData.data[1];
		// Get Motor Direction
		uint8_t direction = i2cData.data[2];
		// Get Motor Speed
		uint8_t speedPercent = i2cData.data[3];
		// Drive Motor
		MOTORDRIVER_setSpeed(motorType, motorLine, direction, speedPercent);
	}
}

