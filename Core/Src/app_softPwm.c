/*
 * app_softPWM.c
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "app_softPwm.h"

#define PWM_SAMPLE	100

static uint32_t pwmBufferPortA[PWM_SAMPLE];
static uint32_t pwmBufferPortB[PWM_SAMPLE];

static bool SOFTPWM_initTimer();
static void SOFTPWM_updateToGpio();
static uint32_t * SOFTPWM_portToBuffer(GPIO_TypeDef * port);

TIM_HandleTypeDef htim3;

bool SOFTPWM_init(){
	// Init Timer for Trigger DMA
	if(!SOFTPWM_initTimer()){
		return false;
	}
	return true;
}

bool SOFTPWM_setDutyCycle(GPIO_TypeDef * port , uint32_t pin, uint8_t dutyCycle){
	// Get dmaBuffer from GPIO Port
	uint32_t *pwmBuffer = SOFTPWM_portToBuffer(port);
	// Fill PWM Data to DMA Buffer at Pin
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

bool SOFTPWW_clearDutyCycle(GPIO_TypeDef * port , uint32_t pin){
	return SOFTPWM_setDutyCycle(port, pin, 0);
}

bool SOFTPWW_clearAllDutyCycle(){
	memset(pwmBufferPortA , 0, sizeof(pwmBufferPortA));
	memset(pwmBufferPortB , 0, sizeof(pwmBufferPortB));
	return true;
}


static bool SOFTPWM_initTimer(){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	// Timer Cycle 0.1ms
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 79;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 4;
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

static uint32_t * SOFTPWM_portToBuffer(GPIO_TypeDef * port){
	if(port == GPIOA){
		return pwmBufferPortA;
	}else if(port == GPIOB){
		return pwmBufferPortB;
	}
	return NULL;
}

static void SOFTPWM_updateToGpio(){
	static uint8_t idx = 0;
	GPIOA->BSRR = pwmBufferPortA[idx];
	GPIOB->BSRR = pwmBufferPortB[idx];
	idx = (idx + 1) % PWM_SAMPLE;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	SOFTPWM_updateToGpio();
}
