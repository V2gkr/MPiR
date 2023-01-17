/*
 * LCD_mo_I2C.h
 *
 *  Created on: 5 квіт. 2022 р.
 *      Author: Поц
 */

#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

/*
 *         PCF8574 <-> HD44780
 *
 * I2C I/O   P7 P6 P5 P4 P3 P2 P1 P0
 * LCD       D7 D6 D5 D4 BL EN RW RS
 *
 * */

extern I2C_HandleTypeDef hi2c1;

#define RS_PIN 0x01
#define RW_PIN 0x02
#define EN_PIN 0x04
#define BL_PIN 0x08

#define INIT_8_BIT_MODE	0x03
#define INIT_4_BIT_MODE	0x02

#define CLEAR_LCD	0x01

#define UNDERLINE_OFF_BLINK_OFF		0x0C
#define UNDERLINE_OFF_BLINK_ON		0x0D
#define UNDERLINE_ON_BLINK_OFF		0x0E
#define UNDERLINE_ON_BLINK_ON		0x0F

#define LINE_1	0x80
#define LINE_2	0xC0

// LCD type define
typedef struct {
	uint8_t addr;
	uint8_t row;
	uint8_t col;
	uint8_t rs_pin;
	uint8_t rw_pin;
	uint8_t en_pin;
	uint8_t bl_pin;
}LCD_InitTypeDef;

// @brief : sends data as a command to LCD
void LCD_Send_Command 	(uint8_t cmd ,LCD_InitTypeDef * LCD_InitStruct);

// @brief : sends data as a data to be displayed on a LCD
void LCD_Send_Data 		(uint8_t cmd ,LCD_InitTypeDef * LCD_InitStruct);

// @brief : Clears the LCD from data
void LCD_Clear 			(LCD_InitTypeDef * LCD_InitStruct);

// @brief : put cursor on a specific address of ddram
void LCD_Set_Cursor		(uint8_t row, uint8_t col,LCD_InitTypeDef * LCD_InitStruct);

// @brief : initialization of a LCD
void LCD_Init 			(LCD_InitTypeDef * LCD_InitStruct);

// @brief : prints data on a LCD
void LCD_Print 			(char *data ,LCD_InitTypeDef * LCD_InitStruct);


#endif /* INC_LCD_MO_I2C_H_ */
