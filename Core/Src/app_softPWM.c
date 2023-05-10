#include "app_softPWM.h"

static uint32_t pwmBufferPortA[PWM_SAMPLE];
static uint32_t pwmBufferPortB[PWM_SAMPLE];

// Internal function
static uint32_t * SOFTPWM_portToBuffer(GPIO_TypeDef * port);

bool SOFTPWM_setDutyCycle(GPIO_TypeDef * port , uint32_t pin, uint8_t dutyCycle){
	uint8_t real_dutyCycle = (uint8_t)(dutyCycle / 5);
	// Get pwmBuffer from GPIO Port
	uint32_t *pwmBuffer = SOFTPWM_portToBuffer(port);
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

bool SOFTPWM_clearDutyCycle(GPIO_TypeDef * port , uint32_t pin){
	return SOFTPWM_setDutyCycle(port, pin, 0);
}

bool SOFTPWM_clearAllDutyCycle(){
	memset(pwmBufferPortA , 0, sizeof(pwmBufferPortA));
	memset(pwmBufferPortB , 0, sizeof(pwmBufferPortB));
	return true;
}


void SOFTPWM_run(){
	static uint8_t idx = 0;
	GPIOA->BSRR = pwmBufferPortA[idx];
	GPIOB->BSRR = pwmBufferPortB[idx];
	idx = (idx + 1) % PWM_SAMPLE;
}

static uint32_t * SOFTPWM_portToBuffer(GPIO_TypeDef * port){
	if(port == GPIOA){
		return pwmBufferPortA;
	}else if(port == GPIOB){
		return pwmBufferPortB;
	}
	return NULL;
}
