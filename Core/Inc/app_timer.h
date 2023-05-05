/*
 * app_timer.h
 *
 *  Created on: May 4, 2023
 *      Author: xuanthodo
 */

#ifndef INC_APP_TIMER_H_
#define INC_APP_TIMER_H_

#include "stdbool.h"

#define TIMER_CYCLE	200 	//microsecond

typedef void(*timer_callback)();

bool TIMER_init();
bool TIMER_attach_callback(timer_callback cb_func);

#endif /* INC_APP_TIMER_H_ */
