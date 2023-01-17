/*
 * PR_INZ.h
 *
 *  Created on: Oct 10, 2022
 *      Author: Поц
 */

#ifndef INC_PR_INZ_H_
#define INC_PR_INZ_H_

#include "stdbool.h"
#include "main.h"
#include "ADS1115.h"
#include "LCD_I2C.h"
#define COUNT_MAX 50
#define TEST
#define LOW 0
#define HIGH 1
#define IN 1
#define OUT 0
#define STANDBY 0
#define FULL_TASK 1
#define MEASUREMENT_ONLY 2
#define MOVING_ONLY 3
#define RETURNING 4
#define END_OF_TASK 5

extern bool itStart_button_flag,
			itStop_button_flag,
			itM1_start_flag,
			itM2_start_flag;
//bool flags to be marked as true to activate some stuff in while loop

typedef struct{
	uint8_t flag_dir; 				// flag for direction in which magnets are moving
	bool M1late_flag;			//late_flag - flags to detect if one of the magnets is late and not synchronized with other one
	bool M2late_flag;
	bool movement_ok_flag; 		//flag which allows to move both or one magnet
	bool M1_start_flag;			// start_flag - flag for detection of start position activation
	bool M2_start_flag;
	bool M1_stop_flag;			// stop-flag - flag for detection of stop position activation
	bool M2_stop_flag;
#ifdef TEST
	uint8_t type_of_task;		//main variable for different tasks
#endif

	uint8_t count; 			// counting of how much steps it did
	char Tx_data[8];			//buffer for sending data
	uint8_t MotorSteps[COUNT_MAX];

}PR_INZ_InitTypeDef;


/* function to start motors */
void PR_INZ_Start();

/* @brief: function to stop motors */
void PR_INZ_Stop();

/*  @brief: makes measurements of field and resistance and then send it via UART to PC and via I2C to LCD*/
void PR_INZ_Measure();

/* @brief: function that force stepper to rotate CW or CCW (depends on flag_dir in PR_INZ_InitTypeDef)*/
void PR_INZ_M1_Move();

void PR_INZ_M2_Move();

/* @moving both magnets at once if you need*/
void PR_INZ_Move_Both(uint8_t steps);

void PR_INZ_Service();
#endif /* INC_PR_INZ_H_ */
