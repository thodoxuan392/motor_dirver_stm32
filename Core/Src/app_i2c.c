/*
 * app_i2c.c
 *
 *  Created on: Feb 1, 2023
 *      Author: xuanthodo
 */

#include "main.h"
#include "app_i2c.h"
#include "stdint.h"

#define	I2C_BUFFER_LEN 	4

I2C_HandleTypeDef hi2c1;

static I2CData_t buffer[I2C_BUFFER_LEN];
static uint8_t buffer_tail = 0;
static uint8_t buffer_head = 0;

// Tx Complete
bool tx_complete = true;

// Internal Function
bool I2C_init(){
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x2000090E;
	hi2c1.Init.OwnAddress1 = 64;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		return false;
	}

	/** Configure Analogue filter
	*/
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return false;
	}

	/** Configure Digital filter
	*/
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		return false;
	}

	if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK)
	{
		return false;
	}
}


bool I2C_run(){

}

bool I2C_receive_data(I2CData_t* data){
	// Check I2C Buffer available
	if(buffer_head == buffer_tail){
		// Not available
		return false;
	}
	* data = buffer[buffer_tail];
	// Increase tail
	buffer_tail = (buffer_tail + 1) % I2C_BUFFER_LEN;
	return true;
}

bool I2C_send_data(I2CData_t *data){
	// Convert to raw data
	uint8_t *rawData = (uint8_t*)data;
	uint8_t rawDataLen = sizeof(I2CData_t);
	// Send Data
	HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, rawData, rawDataLen, I2C_LAST_FRAME);
	return true;
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	switch (TransferDirection) {
		case I2C_DIRECTION_TRANSMIT:
			if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, &buffer, 1, I2C_FIRST_FRAME) != HAL_OK) {
				Error_Handler();
			}
		break;
		case I2C_DIRECTION_RECEIVE:
			if (HAL_I2C_Slave_Seq_Transmit_IT(hi2c, tx_buffer, 1, I2C_LAST_FRAME) != HAL_OK) {
				Error_Handler();
			}
		break;
		default:
			break;
	}
}

// RxCallback
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &rx_temp, 1, I2C_FIRST_AND_NEXT_FRAME) != HAL_OK) {
		Error_Handler();
	}
}

// TxCallback
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	uint8_t temp = tx_buffer[0];
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
	HAL_I2C_EnableListen_IT(hi2c); // Restart
}

