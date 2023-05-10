/*
 * app_motorDriver.c
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#include "main.h"
#include "app_motorDriver.h"
#include "app_dcMotor.h"
#include "app_stepMotor.h"



static IO_t io_table[] = {
	{GPIOA , GPIO_PIN_4}, // Motor A1
	{GPIOB , GPIO_PIN_1},  // Motor A2

	{GPIOA , GPIO_PIN_6}, // Motor B1
	{GPIOA , GPIO_PIN_7},  // Motor B2

	{GPIOA , GPIO_PIN_0}, // Motor C1
	{GPIOA , GPIO_PIN_1},  // Motor C2

	{GPIOA , GPIO_PIN_2}, // Motor D1
	{GPIOA , GPIO_PIN_3}  // Motor D2
};


bool MOTORDRIVER_init(){
	// Enable Clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	// Init GPIO
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	uint8_t nbIo = sizeof(io_table) / sizeof(io_table[0]);
	for (int io_id = 0; io_id < nbIo; ++io_id) {
		GPIO_InitStruct.Pin = io_table[io_id].pin;
		HAL_GPIO_Init(io_table[io_id].port,&GPIO_InitStruct);
	}
}






















































