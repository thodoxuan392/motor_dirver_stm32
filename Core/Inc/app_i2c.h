/*
 * app_i2c.h
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#ifndef INC_APP_I2C_H_
#define INC_APP_I2C_H_

#include "stdbool.h"

typedef struct {
	uint8_t	address;
	uint8_t data[2];
}I2CData_t;


bool I2C_init();

bool I2C_run();

bool I2C_receive_data(I2CData_t *data);

bool I2C_send_data(I2CData_t *data);





#endif /* INC_APP_I2C_H_ */
