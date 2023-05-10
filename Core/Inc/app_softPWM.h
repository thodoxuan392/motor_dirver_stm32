/*
 * app_softPWM.h
 *
 *  Created on: May 10, 2023
 *      Author: xuanthodo
 */

#ifndef INC_APP_SOFTPWM_H_
#define INC_APP_SOFTPWM_H_

#include "main.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"

#define PWM_SAMPLE	100

bool SOFTPWM_setDutyCycle(GPIO_TypeDef * port , uint32_t pin, uint8_t dutyCycle);
bool SOFTPWM_clearDutyCycle(GPIO_TypeDef * port , uint32_t pin);
bool SOFTPWM_clearAllDutyCycle();
void SOFTPWM_run();


#endif /* INC_APP_SOFTPWM_H_ */
