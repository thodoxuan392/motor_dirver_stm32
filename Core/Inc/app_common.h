/*
 * app_common.h
 *
 *  Created on: May 4, 2023
 *      Author: xuanthodo
 */

#ifndef INC_APP_COMMON_H_
#define INC_APP_COMMON_H_

#include "main.h"

typedef struct {
	GPIO_TypeDef* port;
	uint32_t pin;
}IO_t;

enum{
	FORWARD = 0,
	BACKWARD = 1
};

#endif /* INC_APP_COMMON_H_ */
