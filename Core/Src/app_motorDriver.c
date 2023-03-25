/*
 * app_motorDriver.c
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#include "main.h"
#include "app_motorDriver.h"
#include "app_softPwm.h"


typedef struct {
	GPIO_TypeDef* port;
	uint32_t pin;
}IO_t;

typedef struct {
	IO_t motor1;
	IO_t motor2;
}MOTOR_t;

static MOTOR_t motorEntry[] = {
	[MOTOR_A] = {
			{GPIOA , GPIO_PIN_4}, // Motor A1
			{GPIOB , GPIO_PIN_1}  // Motor A2
	},
	[MOTOR_B] = {
			{GPIOA , GPIO_PIN_6}, // Motor B1
			{GPIOA , GPIO_PIN_7}  // Motor B2
	},
	[MOTOR_C] = {
			{GPIOA , GPIO_PIN_0}, // Motor C1
			{GPIOA , GPIO_PIN_1}  // Motor C2
	},
	[MOTOR_D] = {
			{GPIOA , GPIO_PIN_2}, // Motor D1
			{GPIOA , GPIO_PIN_3}  // Motor D2
	},
};

// Internal Function
static MotorLine_t MOTORDRIVER_modifyMotorLine(MotorLine_t motorLine);

bool MOTORDRIVER_init(){
	// Enable Clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	// Init GPIO
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	uint8_t nbMotor = sizeof(motorEntry) / sizeof(motorEntry[0]);
	for (int motorId = 0; motorId < nbMotor; ++motorId) {
		// Init Motor1
		GPIO_InitStruct.Pin = motorEntry[motorId].motor1.pin;
		HAL_GPIO_Init(motorEntry[motorId].motor1.port,&GPIO_InitStruct);
		// Init Motor2
		GPIO_InitStruct.Pin = motorEntry[motorId].motor2.pin;
		HAL_GPIO_Init(motorEntry[motorId].motor2.port,&GPIO_InitStruct);
	}
}



bool MOTORDRIVER_setSpeed(MotorLine_t motorLine, uint8_t direction, uint8_t speedPercent){
	// Check Valid Percent
	if(speedPercent > 100){
		return false;
	}
	// Get MotorLine after Modify
	MotorLine_t new_motorLine = MOTORDRIVER_modifyMotorLine(motorLine);
	// Get Motor from MotorEntry
	MOTOR_t motor =	motorEntry[new_motorLine];
	// Check Direction
	if(direction == FORWARD){
		SOFTPWM_setDutyCycle(motor.motor1.port, motor.motor1.pin, speedPercent);
		SOFTPWM_setDutyCycle(motor.motor2.port, motor.motor2.pin, 0);
	}else{
		SOFTPWM_setDutyCycle(motor.motor1.port, motor.motor1.pin, 0);
		SOFTPWM_setDutyCycle(motor.motor2.port, motor.motor2.pin, speedPercent);
	}
}

// Internal Function
static MotorLine_t MOTORDRIVER_modifyMotorLine(MotorLine_t motorLine){
	MotorLine_t ret_motorLine;
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






















































