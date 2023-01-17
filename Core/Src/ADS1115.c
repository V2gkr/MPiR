/*
 * ADS1115.c
 *
 *  Created on: 5 серп. 2022 р.
 *      Author: Поц
 */

#include "ADS1115.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"



/* @brief: configures ADS1115 how we need
 * ADDRESS and CHANNEL isn't used in configuration and needed only to get channel
 * and to get data from channel what we need */
void ADS_Config(ADS1115_InitTypeDef*ADS1115_InitStruct){
	ADS1115_InitStruct->COMP_LAT 	= 	ADS1115_COMP_LAT_0;
	ADS1115_InitStruct->COMP_MODE 	= 	ADS1115_COMP_MODE_0;
	ADS1115_InitStruct->COMP_POL 	= 	ADS1115_COMP_POL_0;
	ADS1115_InitStruct->DataRate 	= 	ADS1115_DataRate_475;
	ADS1115_InitStruct->MODE 		= 	ADS1115_MODE_ContinuConver;
	//ADS1115_InitStruct->MUX 		= 	ADS1115_MUX_Channel_0;
	ADS1115_InitStruct->OS 			= 	ADS1115_OS_SingleConverStart;
	//ADS1115_InitStruct->PGA 		= 	ADS1115_PGA_4096;
	ADS1115_InitStruct->ADDRESS 	=	ADS1115_ADDRESS_GND;
	//ADS1115_InitStruct->CHANNEL 	= 	ADS1115_CHANNEL0;

	//some objects are in a comments , because they are changeable throughout the project
	uint16_t Config;
	uint8_t Writebuff[3];
	Config = ADS1115_InitStruct->OS + ADS1115_InitStruct->MUX + ADS1115_InitStruct->PGA + ADS1115_InitStruct->MODE + ADS1115_InitStruct->DataRate + ADS1115_InitStruct->COMP_MODE + ADS1115_InitStruct->COMP_POL + ADS1115_InitStruct->COMP_LAT + ADS1115_InitStruct->COMP_QUE;
	Writebuff[0] = ADS1115_Pointer_ConfigReg;
	Writebuff[1] = (unsigned char) ((Config >> 8) & 0xFF);
	Writebuff[2] = (unsigned char) (Config & 0xFF);
	HAL_I2C_Master_Transmit(&hi2c1, ADS1115_InitStruct->ADDRESS|ADS1115_ADDRESS_W, Writebuff, 3,100);
}

/* @brief: gets data like 2 8bit values and connect them in 1 16 bit value */
void ADS1115_ReadRawData(ADS1115_InitTypeDef *ADS1115_InitStruct){
	unsigned char Result[2];
	uint8_t Writebuff[1];
	Writebuff[0] = ADS1115_Pointer_ConverReg;
	HAL_I2C_Master_Transmit(&hi2c1, ADS1115_InitStruct->ADDRESS, Writebuff, 3,100);
	HAL_I2C_Master_Receive(&hi2c1, ADS1115_InitStruct->ADDRESS , Result, 2, 100);
	ADS1115_InitStruct->ADS1115_RawData[ADS1115_InitStruct->CHANNEL] = (int16_t)(((Result[0] << 8) & 0xFF00) | (Result[1] & 0xFF));
}
