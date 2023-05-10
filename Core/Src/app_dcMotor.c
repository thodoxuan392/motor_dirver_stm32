/*
 * app_softPWM.c
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#include <app_dcMotor.h>
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "app_softPWM.h"

static MOTOR_DC_t motorDCEntry[] = {
	[MOTOR_DC_A] = {
			{GPIOA , GPIO_PIN_4}, // Motor A1
			{GPIOB , GPIO_PIN_1}  // Motor A2
	},
	[MOTOR_DC_B] = {
			{GPIOA , GPIO_PIN_6}, // Motor B1
			{GPIOA , GPIO_PIN_7}  // Motor B2
	},
	[MOTOR_DC_C] = {
			{GPIOA , GPIO_PIN_0}, // Motor C1
			{GPIOA , GPIO_PIN_1}  // Motor C2
	},
	[MOTOR_DC_D] = {
			{GPIOA , GPIO_PIN_2}, // Motor D1
			{GPIOA , GPIO_PIN_3}  // Motor D2
	},
};

static bool attach_timer_flag = false;

// Internal function
static void DCMOTOR_run();
static MotorLine_DC_t MOTORDRIVER_DC_modifyMotorLine(MotorLine_DC_t motorLine);

bool DCMOTOR_init(){
	// Do nothing
}

bool DCMOTOR_setSpeed(MotorLine_DC_t motorLine, uint8_t direction, uint16_t speed){
	// Check attached timer interrupt or not
	if(!attach_timer_flag){
		attach_timer_flag = true;
		TIMER_attach_callback(DCMOTOR_run);
	}
	// Check Valid Percent
	if(speed > 100){
		return false;
	}
	// Get MotorLine after Modify
	MotorLine_DC_t new_motorLine = MOTORDRIVER_DC_modifyMotorLine(motorLine);
	// Get Motor from MotorEntry
	MOTOR_DC_t motor =	motorDCEntry[new_motorLine];
	// Check Direction
	if(direction == FORWARD){
		SOFTPWM_setDutyCycle(motor.motor1.port, motor.motor1.pin, speed);
		SOFTPWM_setDutyCycle(motor.motor2.port, motor.motor2.pin, 0);
	}else{
		SOFTPWM_setDutyCycle(motor.motor1.port, motor.motor1.pin, 0);
		SOFTPWM_setDutyCycle(motor.motor2.port, motor.motor2.pin, speed);
	}
}


static MotorLine_DC_t MOTORDRIVER_DC_modifyMotorLine(MotorLine_DC_t motorLine){
	MotorLine_DC_t ret_motorLine;
	switch (motorLine) {
		case 0:
			ret_motorLine = 1;
			break;
		case 1:
			ret_motorLine = 0;
			break;
		case 2:
			ret_motorLine = 3;
			break;
		case 3:
			ret_motorLine = 2;
			break;
		default:
			break;
	}
	return ret_motorLine;
}

static void DCMOTOR_run(){
	SOFTPWM_run();
}


