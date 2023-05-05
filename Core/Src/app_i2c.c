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

enum {
	STATE_I2C_GET_DATA,
	STATE_I2C_STOP
};

I2C_HandleTypeDef hi2c1;

// I2C Buffer
static I2CData_t buffer[I2C_BUFFER_LEN];
static uint8_t buffer_tail = 0;
static uint8_t buffer_head = 0;
static uint8_t buffer_idx = 0;

// I2C State
static uint8_t i2c_state;

// Tx Temporatory
uint8_t tx_temp[3] = {1,2,3,4};

// Internal Function
bool I2C_init(){
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x2000090E;
	hi2c1.Init.OwnAddress1 = 96;
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
	return true;
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
	HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, rawData, rawDataLen, I2C_LAST_FRAME);
	return true;
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	switch (TransferDirection) {
		case I2C_DIRECTION_TRANSMIT:
			// Reset I2C Data State
			i2c_state = STATE_I2C_GET_DATA;
			// Reset Idx for I2C Data
			buffer_idx = 1;
			if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, &buffer[buffer_head].data[0], 1, I2C_FIRST_FRAME) != HAL_OK) {
				Error_Handler();
			}
			break;
		case I2C_DIRECTION_RECEIVE:
			if (HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &tx_temp, 1, I2C_LAST_FRAME) != HAL_OK) {
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
	if(i2c_state == STATE_I2C_GET_DATA){
		switch (buffer_idx) {
			case 1:
				// Get Data Byte 1
				if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, &buffer[buffer_head].data[buffer_idx], 1, I2C_NEXT_FRAME) != HAL_OK) {
					Error_Handler();
				}
				break;
			case 2:
				// Get Data Byte 2
				if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, &buffer[buffer_head].data[buffer_idx], 1, I2C_NEXT_FRAME) != HAL_OK) {
					Error_Handler();
				}
				break;
			case 3:
				// Get Data Byte 3
				if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, &buffer[buffer_head].data[buffer_idx], 1, I2C_LAST_FRAME) != HAL_OK) {
					Error_Handler();
				}
				break;
			default:
				break;
		}
		buffer_idx++;
		if(buffer_idx >= sizeof(I2CData_t) + 1){
			buffer_head = (buffer_head + 1) % I2C_BUFFER_LEN;
			i2c_state = STATE_I2C_STOP;
		}
	}
}

// TxCallback
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
	if(HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK)
	{
		/* Transfer error in reception process */
		Error_Handler();
	}
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
	/** Error_Handler() function is called when error occurs.
	* 1- When Slave doesn't acknowledge its address, Master restarts communication.
	* 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
	*/
}

