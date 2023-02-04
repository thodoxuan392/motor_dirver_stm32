/*
 * app_softPWM.c
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#include "main.h"
#include "app_softPwm.h"

#define PWM_SAMPLE	100

static uint32_t dmaBufferPortA[PWM_SAMPLE];
static uint32_t dmaBufferPortB[PWM_SAMPLE];

static bool SOFTPWM_initTimer();
static bool SOFTPWM_initDMA();

static uint32_t * SOFTPWM_portToDMABuffer(GPIO_TypeDef * port);

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1;

bool SOFTPWM_init(){
	// Init Timer for Trigger DMA
	SOFTPWM_initTimer();

	// Init DMA
	SOFTPWM_initDMA();

	// Start Timer
	HAL_TIM_Base_Start(&htim1);

	// Start DMA
//	HAL_DMA_Start(hdma, SrcAddress, DstAddress, DataLength);

	// Enable DMA with Timer Interrupt
	__HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);

}

bool SOFTPWM_setDutyCycle(GPIO_TypeDef * port , uint32_t pin, uint8_t dutyCycle);

bool SOFTPWW_clearDutyCycle(GPIO_TypeDef * port , uint32_t pin){
	SOFTPWM_setDutyCycle(port, pin, 0);
}

bool SOFTPWW_clearAllDutyCycle(){
	memset(dmaBufferPortA , 0, sizeof(dmaBufferPortA));
	memset(dmaBufferPortB , 0, sizeof(dmaBufferPortB));
}


static bool SOFTPWM_initTimer(){

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 99;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		return false;
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
		return false;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
		return false;
	}
	return true;

}
static bool SOFTPWM_initDMA(){
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* Configure DMA request hdma_memtomem_dma1_channel1 on DMA1_Channel1 */
	hdma_tim1.Instance = DMA1_Channel1;
	hdma_tim1.Init.Direction = DMA_MEMORY_TO_MEMORY;
	hdma_tim1.Init.PeriphInc = DMA_PINC_ENABLE;
	hdma_tim1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tim1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_tim1.Init.Mode = DMA_NORMAL;
	hdma_tim1.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_tim1) != HAL_OK)
	{
		return false;
	}
	return true;
}

static uint32_t * SOFTPWM_portToDMABuffer(GPIO_TypeDef * port){
	if(port == GPIOA) {
		return dmaBufferPortA;
	}else if (port == GPIOB){
		return dmaBufferPortB;
	}
}

