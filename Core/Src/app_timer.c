/*
 * app_timer.c
 *
 *  Created on: May 4, 2023
 *      Author: xuanthodo
 */

#include "main.h"
#include "app_timer.h"

#define FN_CALLBACK_MAX	4

TIM_HandleTypeDef htim3;

static timer_callback fn_callback = NULL;

bool TIMER_init(){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	// Timer Cycle 0.1ms```````````
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 8 * TIMER_CYCLE / 20 - 1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 20;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		return false;
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		return false;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		return false;
	}

	// Start Timer Interrupt
	if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK){
		return false;
	}
	return true;
}

bool TIMER_attach_callback(timer_callback cb_func){
	fn_callback = cb_func;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(fn_callback)
		fn_callback();
}


