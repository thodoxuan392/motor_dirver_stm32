/*
 * app_stepMotor.c
 *
 *  Created on: May 4, 2023
 *      Author: xuanthodo
 */
#include "app_stepMotor.h"
#include "app_timer.h"

typedef struct {
	bool enable;
	uint8_t direction;
	uint32_t counter_delay;
	uint32_t counter_index;
	uint8_t current_step;
}STEPMOTOR_info_t;

static uint32_t step_delay_microsecond = 1000;	// 1 miliseconds
static bool attach_timer_flag = false;

static STEPMOTOR_info_t stepMotor_state_table[MOTOR_STEP_MAX] = {
	[MOTOR_STEP_1] = {
			.enable = false,
			.counter_delay = 0,
			.counter_index = 0,
			.current_step =0,
	},
	[MOTOR_STEP_2] = {
			.enable = false,
			.counter_delay = 0,
			.counter_index = 0,
			.current_step =0,
	},
};

static MOTOR_Stepper_t stepMotor_io_table[] = {
	[MOTOR_STEP_1] = {
			{GPIOA , GPIO_PIN_4}, // Motor 1
			{GPIOB , GPIO_PIN_1}, // Motor 2
			{GPIOA , GPIO_PIN_6}, // Motor 3
			{GPIOA , GPIO_PIN_7}  // Motor 4
	},
	[MOTOR_STEP_2] = {
			{GPIOA , GPIO_PIN_0}, // Motor 1
			{GPIOA , GPIO_PIN_1}, // Motor 2
			{GPIOA , GPIO_PIN_2}, // Motor 3
			{GPIOA , GPIO_PIN_3}  // Motor 4
	},
};

// Internal function
static void STEPMOTOR_run();

bool STEPMOTOR_init(){
	// Do nothing
}

bool STEPMOTOR_setSpeed(MotorLine_Step_t motorLine, uint8_t direction, uint8_t speed){
	// Check attached timer interrupt or not
	if(!attach_timer_flag){
		attach_timer_flag = true;
		TIMER_attach_callback(STEPMOTOR_run);
	}
	// Check Stepper line
	if(motorLine >= MOTOR_STEP_MAX){
		return false;
	}
	if(speed == 0){
		stepMotor_state_table[motorLine].enable = false;
	}else{
		stepMotor_state_table[motorLine].enable = true;
		step_delay_microsecond = 60000000 / NUMBER_OF_STEP / speed;
		stepMotor_state_table[motorLine].direction = direction;
		stepMotor_state_table[motorLine].counter_delay = step_delay_microsecond / TIMER_CYCLE;
		stepMotor_state_table[motorLine].counter_index = 0;
		stepMotor_state_table[motorLine].current_step = 0;
	}
}

static void STEPMOTOR_run(){
	for (int index = 0; index < MOTOR_STEP_MAX; ++index) {
		if(stepMotor_state_table[index].enable){
			stepMotor_state_table[index].counter_index ++;
			if(stepMotor_state_table[index].counter_index >= stepMotor_state_table[index].counter_delay) {
				switch (stepMotor_state_table[index].current_step) {
				  case 0:  // 1010
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor1.port,
										stepMotor_io_table[index].motor1.pin,
										SET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor2.port,
										stepMotor_io_table[index].motor2.pin,
										RESET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor3.port,
										stepMotor_io_table[index].motor3.pin,
										SET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor4.port,
										stepMotor_io_table[index].motor4.pin,
										RESET);
				  break;
				  case 1:  // 0110
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor1.port,
										stepMotor_io_table[index].motor1.pin,
										RESET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor2.port,
										stepMotor_io_table[index].motor2.pin,
										SET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor3.port,
										stepMotor_io_table[index].motor3.pin,
										SET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor4.port,
										stepMotor_io_table[index].motor4.pin,
										RESET);
				  break;
				  case 2:  //0101
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor1.port,
										stepMotor_io_table[index].motor1.pin,
										RESET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor2.port,
										stepMotor_io_table[index].motor2.pin,
										SET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor3.port,
										stepMotor_io_table[index].motor3.pin,
										RESET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor4.port,
										stepMotor_io_table[index].motor4.pin,
										SET);
				  break;
				  case 3:  //1001
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor1.port,
										stepMotor_io_table[index].motor1.pin,
										SET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor2.port,
										stepMotor_io_table[index].motor2.pin,
										RESET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor3.port,
										stepMotor_io_table[index].motor3.pin,
										RESET);
					HAL_GPIO_WritePin(stepMotor_io_table[index].motor4.port,
										stepMotor_io_table[index].motor4.pin,
										SET);
				  break;
				}
				stepMotor_state_table[index].counter_index = 0;
				// Forward
				if(stepMotor_state_table[index].direction){
					stepMotor_state_table[index].current_step = (stepMotor_state_table[index].current_step + 1) % 4;
				}
				// Backward
				else {
					if(stepMotor_state_table[index].current_step == 0){
						stepMotor_state_table[index].current_step = 3;
					}else{
						stepMotor_state_table[index].current_step--;
					}
				}
			}
		}
	}

}

