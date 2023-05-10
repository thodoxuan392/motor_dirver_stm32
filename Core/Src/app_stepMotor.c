/*
 * app_stepMotor.c
 *
 *  Created on: May 4, 2023
 *      Author: xuanthodo
 */
#include "app_stepMotor.h"
#include "app_timer.h"

#define isSET(reg,index)		(reg >> index) & 0x01

typedef struct {
	bool enable;
	MOTOR_Stepper_t io;
	uint8_t direction;
	uint16_t number_step;
	StepperStyle_t style;
	StepperMode_t mode;
	uint32_t counter_delay;
	uint32_t counter_index;
	uint16_t current_step;
	uint16_t run_step;
	uint16_t max_step;
}STEPMOTOR_info_t;

static uint32_t step_delay_microsecond = 1000;	// 1 miliseconds
static bool attach_timer_flag = false;

static STEPMOTOR_info_t stepMotor_state_table[MOTOR_STEP_MAX] = {
	[MOTOR_STEP_1] = {
			.io = {
					{GPIOA , GPIO_PIN_4}, // Motor 1
					{GPIOB , GPIO_PIN_1}, // Motor 2
					{GPIOA , GPIO_PIN_6}, // Motor 3
					{GPIOA , GPIO_PIN_7}  // Motor 4
			},
			.enable = false,
			.counter_delay = 0,
			.counter_index = 0,
			.current_step =0,
	},
	[MOTOR_STEP_2] = {
			.io = {
					{GPIOA , GPIO_PIN_0}, // Motor 1
					{GPIOA , GPIO_PIN_1}, // Motor 2
					{GPIOA , GPIO_PIN_2}, // Motor 3
					{GPIOA , GPIO_PIN_3}  // Motor 4
			},
			.enable = false,
			.counter_delay = 0,
			.counter_index = 0,
			.current_step = 0,
	},
};

static uint8_t style_table[STYLE_MAX][MICRO_STEP_MAX] = {
		[SINGLE] = {
				0b0010,
				0b0001
		},
		[DOUBLE] = {
				0b1010,
				0b0110,
				0b0101,
				0b1001
		},
		[INTERLEAVE] = {
				0b0010,
				0b0110,
				0b0100,
				0b0101,
				0b0001,
				0b1001,
				0b1000,
				0b1010
		},
		[MICROSTEP] = {
				0b1010,
				0b0110,
				0b0101,
				0b1001
		}
};

// Internal function
static void STEPMOTOR_run();
static void STEPMOTOR_runStep(STEPMOTOR_info_t * motor);
static void STEPMOTOR_runPwm(STEPMOTOR_info_t * motor);

bool STEPMOTOR_init(){
	// Do nothing
}

bool STEPMOTOR_setSpeed(MotorLine_Step_t motorLine, uint16_t number_step, uint8_t direction, uint16_t speed, StepperStyle_t style){
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
		stepMotor_state_table[motorLine].number_step = number_step;
		stepMotor_state_table[motorLine].direction = direction;
		stepMotor_state_table[motorLine].mode = SPEED;
		stepMotor_state_table[motorLine].style = style;
		step_delay_microsecond = 60000000 / number_step / speed;
		stepMotor_state_table[motorLine].counter_delay = step_delay_microsecond / TIMER_CYCLE;
		stepMotor_state_table[motorLine].counter_index = 0;
		stepMotor_state_table[motorLine].current_step = 0;
		switch (style) {
			case SINGLE:
				stepMotor_state_table[motorLine].max_step = SINGLE_STEP_MAX;
				break;
			case DOUBLE:
				stepMotor_state_table[motorLine].max_step = DOUBLE_STEP_MAX;
				break;
			case INTERLEAVE:
				stepMotor_state_table[motorLine].max_step = INTERLEAVE_STEP_MAX;
				break;
			case MICROSTEP:
				// Use PWM to run
				break;
			default:
				break;
		}
	}
}


bool STEPMOTOR_step(MotorLine_Step_t motorLine, uint16_t number_step, uint8_t direction, uint16_t step, StepperStyle_t style){
	// Check attached timer interrupt or not
	if(!attach_timer_flag){
		attach_timer_flag = true;
		TIMER_attach_callback(STEPMOTOR_run);
	}
	// Check Stepper line
	if(motorLine >= MOTOR_STEP_MAX){
		return false;
	}
	if(step == 0){
		stepMotor_state_table[motorLine].enable = false;
	}else{
		stepMotor_state_table[motorLine].enable = true;
		stepMotor_state_table[motorLine].number_step = number_step;
		stepMotor_state_table[motorLine].direction = direction;
		stepMotor_state_table[motorLine].mode = STEP;
		stepMotor_state_table[motorLine].style = style;
		step_delay_microsecond = 60000000 / number_step / SPEED_DEFAULT;
		stepMotor_state_table[motorLine].counter_delay = step_delay_microsecond / TIMER_CYCLE;
		stepMotor_state_table[motorLine].counter_index = 0;
		stepMotor_state_table[motorLine].current_step = 0;
		stepMotor_state_table[motorLine].run_step = step;
		switch (style) {
			case SINGLE:
				stepMotor_state_table[motorLine].max_step = SINGLE_STEP_MAX;
				break;
			case DOUBLE:
				stepMotor_state_table[motorLine].max_step = DOUBLE_STEP_MAX;
				break;
			case INTERLEAVE:
				stepMotor_state_table[motorLine].max_step = INTERLEAVE_STEP_MAX;
				break;
			case MICROSTEP:
				// Use PWM to run
				break;
			default:
				break;
		}
	}
}

static void STEPMOTOR_run(){
	for (int index = 0; index < MOTOR_STEP_MAX; ++index) {
		if(stepMotor_state_table[index].enable){
			STEPMOTOR_runStep(&stepMotor_state_table[index]);
		}
	}
}

static void STEPMOTOR_runStep(STEPMOTOR_info_t * motor){
	motor->counter_index ++;
	if(motor->counter_index >= motor->counter_delay) {
		HAL_GPIO_WritePin(motor->io.motor1.port,
							motor->io.motor1.pin,
							isSET(style_table[motor->style][motor->current_step],3));
		HAL_GPIO_WritePin(motor->io.motor2.port,
							motor->io.motor2.pin,
							isSET(style_table[motor->style][motor->current_step],2));
		HAL_GPIO_WritePin(motor->io.motor3.port,
							motor->io.motor3.pin,
							isSET(style_table[motor->style][motor->current_step],1));
		HAL_GPIO_WritePin(motor->io.motor4.port,
							motor->io.motor4.pin,
							isSET(style_table[motor->style][motor->current_step],0));
		motor->counter_index = 0;
		// Forward
		if(motor->direction){
			motor->current_step = (motor->current_step + 1) % motor->max_step;
		}
		// Backward
		else {
			if(motor->current_step == 0){
				motor->current_step = motor->max_step - 1;
			}else{
				motor->current_step--;
			}
		}
		// Check if is running STEP mode
		if(motor->mode == STEP){
			// If run_step is reached -> Stop motor
			motor->run_step --;
			if(motor->run_step == 0){
				motor->enable = false;
			}
		}
	}
}

