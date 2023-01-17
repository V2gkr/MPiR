/*
 * LCD_mo_I2C.c
 *
 *  Created on: 5 квіт. 2022 р.
 *      Author: Поц
 */

#include "LCD_I2C.h"
#include "stm32f4xx_hal.h"
#include "i2c.h"

void LCD_Send_Command (uint8_t cmd ,LCD_InitTypeDef * LCD_InitStruct)
{
	uint8_t data_t[4];
	data_t[0] = (cmd&0xf0)|EN_PIN|LCD_InitStruct->bl_pin;  //en=1, rs=0
	data_t[1] = (cmd&0xf0)|LCD_InitStruct->bl_pin;  //en=0, rs=0
	data_t[2] = cmd<<4|LCD_InitStruct->en_pin|LCD_InitStruct->bl_pin;  //en=1, rs=0
	data_t[3] = cmd<<4|LCD_InitStruct->bl_pin;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c1, LCD_InitStruct->addr,(uint8_t *) data_t, 4,100);
	HAL_Delay(5);
}
void LCD_Send_Data (uint8_t cmd ,LCD_InitTypeDef * LCD_InitStruct)
{

	uint8_t data_t[4];
	data_t[0] = (cmd&0xf0)|EN_PIN|LCD_InitStruct->bl_pin|LCD_InitStruct->rs_pin;  //en=1, rs=0
	data_t[1] = (cmd&0xf0)|LCD_InitStruct->bl_pin|LCD_InitStruct->rs_pin;  //en=0, rs=0
	data_t[2] = cmd<<4|LCD_InitStruct->en_pin|LCD_InitStruct->bl_pin|LCD_InitStruct->rs_pin;  //en=1, rs=0
	data_t[3] = cmd<<4|LCD_InitStruct->bl_pin|LCD_InitStruct->rs_pin;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c1, LCD_InitStruct->addr,(uint8_t *) data_t, 4,100);
	HAL_Delay(5);
}

void LCD_Clear (LCD_InitTypeDef * LCD_InitStruct)
{
	LCD_Send_Command(CLEAR_LCD,LCD_InitStruct);
}

void LCD_Set_Cursor(uint8_t row, uint8_t col,LCD_InitTypeDef * LCD_InitStruct)
{

    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0x80|0x40;
            break;
        case 2:
            col |= 0x80|0x14;
            break;
        case 3:
            col |= 0xC0|0x54;
            break;
    }// rows - от 1 до 3 - выбор ряда дисплея , cols - смотреть в даташите и отдавать шестнадцатеричные адреса

    LCD_Send_Command (col, LCD_InitStruct);
}


void LCD_Init (LCD_InitTypeDef * LCD_InitStruct){

	LCD_InitStruct->addr=0x27<<1;
	LCD_InitStruct->bl_pin=BL_PIN;
	LCD_InitStruct->en_pin=EN_PIN;
	LCD_InitStruct->rs_pin=RS_PIN;
	LCD_InitStruct->rw_pin=RW_PIN;
	// initialisation
	HAL_Delay(40);  // wait for >40ms
	LCD_Send_Command (INIT_8_BIT_MODE,LCD_InitStruct);
	HAL_Delay(5);  // wait for >4.1ms
	LCD_Send_Command (INIT_8_BIT_MODE,LCD_InitStruct);
	HAL_Delay(1);  // wait for >100us
	LCD_Send_Command (INIT_8_BIT_MODE,LCD_InitStruct);
	//4 bit initialisation
	LCD_Send_Command (INIT_4_BIT_MODE,LCD_InitStruct);

	LCD_Send_Command (UNDERLINE_OFF_BLINK_OFF,LCD_InitStruct); //disp on/off
	HAL_Delay(10);
	LCD_Clear(LCD_InitStruct);
}

void LCD_Print (char *data ,LCD_InitTypeDef * LCD_InitStruct)
{

	while(*data)
		{

			LCD_Send_Data (*data++,LCD_InitStruct);
		}
}

