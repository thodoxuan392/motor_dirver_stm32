/*
 * app_softPWM.h
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#ifndef INC_APP_SOFTPWM_H_
#define INC_APP_SOFTPWM_H_

#include "stdbool.h"

bool SOFTPWM_init();

bool SOFTPWM_setDutyCycle(GPIO_TypeDef * port , uint32_t pin, uint8_t dutyCycle);

bool SOFTPWW_clearDutyCycle(GPIO_TypeDef * port , uint32_t pin);

bool SOFTPWW_clearAllDutyCycle();


#endif /* INC_APP_SOFTPWM_H_ */
