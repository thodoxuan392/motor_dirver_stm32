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
#include "app_timer.h"

#define PWM_SAMPLE	100

static uint32_t pwmBufferPortA[PWM_SAMPLE];
static uint32_t pwmBufferPortB[PWM_SAMPLE];


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
static uint32_t * DCMOTOR_portToBuffer(GPIO_TypeDef * port);
static bool DCMOTOR_setDutyCycle(GPIO_TypeDef * port , uint32_t pin, uint8_t dutyCycle);
static bool DCMOTOR_clearDutyCycle(GPIO_TypeDef * port , uint32_t pin);
static bool DCMOTOR_clearAllDutyCycle();
static MotorLine_DC_t MOTORDRIVER_DC_modifyMotorLine(MotorLine_DC_t motorLine);

bool DCMOTOR_init(){
	// Do nothing
}

bool DCMOTOR_setSpeed(MotorLine_DC_t motorLine, uint8_t direction, uint8_t speed){
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
		DCMOTOR_setDutyCycle(motor.motor1.port, motor.motor1.pin, speed);
		DCMOTOR_setDutyCycle(motor.motor2.port, motor.motor2.pin, 0);
	}else{
		DCMOTOR_setDutyCycle(motor.motor1.port, motor.motor1.pin, 0);
		DCMOTOR_setDutyCycle(motor.motor2.port, motor.motor2.pin, speed);
	}
}


// Internal function
static bool DCMOTOR_setDutyCycle(GPIO_TypeDef * port , uint32_t pin, uint8_t dutyCycle){
	uint8_t real_dutyCycle = (uint8_t)(dutyCycle / 5);
	// Get pwmBuffer from GPIO Port
	uint32_t *pwmBuffer = DCMOTOR_portToBuffer(port);
	// Fill PWM Data
	for (uint32_t i = 0;  i < PWM_SAMPLE; ++ i) {
		if(i < dutyCycle){
			// Set Pin
			pwmBuffer[i]&=(uint32_t)~(pin<<16);
			pwmBuffer[i]|=(uint32_t)pin;
		}else{
			// Reset pin
			pwmBuffer[i]&=(uint32_t)~(pin);
			pwmBuffer[i]|=(uint32_t)pin<<16;
		}
	}
	return true;
}

static bool DCMOTOR_clearDutyCycle(GPIO_TypeDef * port , uint32_t pin){
	return DCMOTOR_setDutyCycle(port, pin, 0);
}

static bool DCMOTOR_clearAllDutyCycle(){
	memset(pwmBufferPortA , 0, sizeof(pwmBufferPortA));
	memset(pwmBufferPortB , 0, sizeof(pwmBufferPortB));
	return true;
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


static uint32_t * DCMOTOR_portToBuffer(GPIO_TypeDef * port){
	if(port == GPIOA){
		return pwmBufferPortA;
	}else if(port == GPIOB){
		return pwmBufferPortB;
	}
	return NULL;
}

static void DCMOTOR_run(){
	static uint8_t idx = 0;
	GPIOA->BSRR = pwmBufferPortA[idx];
	GPIOB->BSRR = pwmBufferPortB[idx];
	idx = (idx + 1) % PWM_SAMPLE;
}


