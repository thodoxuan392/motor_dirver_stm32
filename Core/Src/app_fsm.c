/*
 * app_fsm.c
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#include "stdio.h"
#include "app_fsm.h"
#include "app_i2c.h"
#include "app_motorDriver.h"
#include "app_dcMotor.h"
#include "app_stepMotor.h"

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
		switch (motorType) {
			case DC_MOTOR:
				FSM_dcMotor(&i2cData);
				break;
			case STEP_MOTOR:
				FSM_stepperMotor(&i2cData);
				break;
			default:
				break;
		}
	}
}


void FSM_dcMotor(I2CData_t *data){
	uint8_t motor_index = data->data[1];
	uint8_t dir = data->data[2];
	uint16_t speed = ((uint16_t)(data->data[3]) << 8 ) | data->data[4];
	DCMOTOR_setSpeed( motor_index,dir,speed);
}

void FSM_stepperMotor(I2CData_t *data){
	uint8_t motor_index = data->data[1];
	uint16_t number_step = ((uint16_t)(data->data[2]) << 8) | data->data[3];
	uint8_t style = data->data[4];
	uint8_t mode = data->data[5];
	uint8_t dir = data->data[6];
	uint16_t stepOrSpeed = ((uint16_t)(data->data[7]) << 8) | data->data[8];
	switch (mode) {
		case SPEED:
			STEPMOTOR_setSpeed(motor_index, number_step, dir, stepOrSpeed, style);
			break;
		case STEP:
			STEPMOTOR_step(motor_index, number_step, dir, stepOrSpeed, style);
			break;
		default:
			break;
	}
}

