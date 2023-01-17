/*
 * PR_INZ.c
 *
 *  Created on: Oct 10, 2022
 *      Author: Поц
 */

#include "PR_INZ.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "stdio.h"
#include "usart.h"
#include "NVIC_Service.h"

bool itStart_button_flag=0,
	 itStop_button_flag=0,
	 itM1_start_flag=0,
	 itM2_start_flag=0;
	//bool flags to be marked as true to activate some stuff in while loop

extern ADS1115_InitTypeDef ADS1115;
extern LCD_InitTypeDef 	LCD1;
extern PR_INZ_InitTypeDef 	PRINZ1;

/* @brief: function to start motors */
void PR_INZ_Start(){

		 HAL_TIM_Base_Start_IT(&htim10);
		 PRINZ1.flag_dir=IN;
		 PRINZ1.movement_ok_flag=HIGH;
		 PRINZ1.count=0;
		 PRINZ1.M1late_flag=0;
		 PRINZ1.M2late_flag=0;
		 GPIOA->BSRR=LED_START_Pin;
		 GPIOA->BSRR=(uint32_t)LED_STOP_Pin<<16;
		 ucUART_1byteflag=2;
}

/* @brief: function to stop motors */
void PR_INZ_Stop(){
		HAL_TIM_Base_Stop_IT(&htim10);
		//PRINZ1.flag_dir=IN;
		PRINZ1.movement_ok_flag=LOW;
		PRINZ1.M1_start_flag=0;
		PRINZ1.M1_stop_flag=0;
		PRINZ1.M2_start_flag=0;
		PRINZ1.M2_stop_flag=0;
		GPIOA->BSRR=LED_START_Pin;
		GPIOA->BSRR=(uint32_t)LED_STOP_Pin<<16;
}

/*  @brief: makes measurements of field and resistance and then send it via UART to PC and via I2C to LCD*/
void PR_INZ_Measure(){
	LCD_Clear(&LCD1);
	if(ucUART_1byteflag==0 || ucUART_1byteflag==2){
	ADS1115.CHANNEL 	= 	ADS1115_CHANNEL0;
	ADS1115.MUX 		= 	ADS1115_MUX_Channel_0;
	ADS1115.PGA 		= 	ADS1115_PGA_4096;
	ADS_Config(&ADS1115);
	HAL_Delay(10);
	ADS1115_ReadRawData(&ADS1115);
	sprintf(PRINZ1.Tx_data,"Vf=%d V",ADS1115.ADS1115_RawData[0]);
	HAL_UART_Transmit_DMA(&huart2,(uint8_t*)PRINZ1.Tx_data,8);
	LCD_Set_Cursor(0, 0,&LCD1);
	LCD_Print(PRINZ1.Tx_data, &LCD1);
	}
	if(ucUART_1byteflag==1 || ucUART_1byteflag==2){
	ADS1115.CHANNEL 	= 	ADS1115_CHANNEL1;
	ADS1115.MUX 		= 	ADS1115_MUX_Differ_13;
	ADS1115.PGA 		= 	ADS1115_PGA_0512;
	ADS_Config(&ADS1115);
	HAL_Delay(10);
	ADS1115_ReadRawData(&ADS1115);

	sprintf(PRINZ1.Tx_data,"Vr=%d V",ADS1115.ADS1115_RawData[1]);
	HAL_UART_Transmit_DMA(&huart2,(uint8_t*)PRINZ1.Tx_data,8);
	LCD_Set_Cursor(1, 0,&LCD1);
	LCD_Print(PRINZ1.Tx_data, &LCD1);
	}
	if(PRINZ1.flag_dir && PRINZ1.movement_ok_flag) {
		LCD_Set_Cursor(0, 10,&LCD1);
		LCD_Print("meas+",&LCD1);
	}
	else if (PRINZ1.flag_dir==OUT && PRINZ1.movement_ok_flag){
		LCD_Set_Cursor(0, 10, &LCD1);
		LCD_Print("meas-", &LCD1);
	}
}

/* @brief: function that force stepper to rotate CW or CCW (depends on flag_dir state)
 	 count will be changed in that function*/

void PR_INZ_M1_Move(){
	HAL_GPIO_WritePin(M1_direction_GPIO_Port,M1_direction_Pin, PRINZ1.flag_dir);

	for(uint8_t i=0;i<PRINZ1.MotorSteps[PRINZ1.count];i++){

		GPIOB->BSRR=M1_step_Pin;
		HAL_Delay(1);
		GPIOB->BSRR=(uint32_t)M1_step_Pin<<16U;
		HAL_Delay(1);

	}
}
/* @brief: new function for moving both a the time , used only from uart call , */
void PR_INZ_Move_Both(uint8_t steps){
	HAL_GPIO_WritePin(M1_direction_GPIO_Port,M1_direction_Pin, PRINZ1.flag_dir);
	HAL_GPIO_WritePin(M2_direction_GPIO_Port,M2_direction_Pin, PRINZ1.flag_dir);
	for(uint8_t i=0;i<steps;i++){
	GPIOB->BSRR=M1_step_Pin|M2_step_Pin;
	HAL_Delay(1);
	GPIOB->BSRR=(uint32_t)(M1_step_Pin|M2_step_Pin)<<16U;
	HAL_Delay(1);
	}
}
/* @brief: function that force stepper to rotate CW or CCW (depends on flag_dir state)
 * continue_flag will be changed in that function as a
 */
void PR_INZ_M2_Move(){
	HAL_GPIO_WritePin(M2_direction_GPIO_Port,M2_direction_Pin, PRINZ1.flag_dir);
	for(uint8_t i=0;i<PRINZ1.MotorSteps[PRINZ1.count];i++){
		GPIOB->BSRR=M2_step_Pin;
		HAL_Delay(1);
		GPIOB->BSRR=(uint32_t)M2_step_Pin<<16U;
		HAL_Delay(1);
	}
}

/*@brief: task for servising all gpio activations*/
void PR_INZ_Service(){

	switch(PRINZ1.type_of_task){
	case FULL_TASK:
		if(PRINZ1.flag_dir==IN &&(PRINZ1.count!=COUNT_MAX-1)&&PRINZ1.movement_ok_flag){
			if(PRINZ1.M1late_flag && !PRINZ1.M2late_flag) {
				PR_INZ_M1_Move();
			}
			else if(PRINZ1.M2late_flag && !PRINZ1.M1late_flag) {
				PR_INZ_M2_Move();
			}
			else if(!(PRINZ1.M1late_flag && PRINZ1.M2late_flag)) {
				PR_INZ_Move_Both(PRINZ1.MotorSteps[PRINZ1.count]);
				PR_INZ_Measure();
				PRINZ1.count++;
			}
			PRINZ1.movement_ok_flag=0;
		}
		else if(PRINZ1.flag_dir==OUT &&(PRINZ1.count!=0)&&PRINZ1.movement_ok_flag){
			if(PRINZ1.M1late_flag && !PRINZ1.M2late_flag) {
				PR_INZ_M1_Move();
			}
			else if(PRINZ1.M2late_flag && !PRINZ1.M1late_flag) {
				PR_INZ_M2_Move();
			}
			else if(!(PRINZ1.M1late_flag && PRINZ1.M2late_flag)) {
				PR_INZ_Move_Both(PRINZ1.MotorSteps[PRINZ1.count]);
				PR_INZ_Measure();
				PRINZ1.count--;
			}
			PRINZ1.movement_ok_flag=0;
		}
		if(PRINZ1.count==COUNT_MAX-1) PRINZ1.flag_dir=OUT;
		else if(PRINZ1.count==0 &&PRINZ1.M1_stop_flag&&PRINZ1.M2_stop_flag){
			PRINZ1.type_of_task=END_OF_TASK;
		}
		break;

	case MEASUREMENT_ONLY:
		PR_INZ_Measure();//we only measuring here
		PRINZ1.type_of_task=STANDBY;
		break;

	case MOVING_ONLY:
		if(PRINZ1.flag_dir==IN &&(PRINZ1.count!=COUNT_MAX-1)&&PRINZ1.movement_ok_flag){
			if(PRINZ1.M1late_flag && !PRINZ1.M2late_flag) {
				PR_INZ_M1_Move();
			}
			else if(PRINZ1.M2late_flag && !PRINZ1.M1late_flag) {
				PR_INZ_M2_Move();
			}
			else if(!(PRINZ1.M1late_flag && PRINZ1.M2late_flag)) {
				PR_INZ_Move_Both(PRINZ1.MotorSteps[PRINZ1.count]);
				PRINZ1.count++;
			}
			PRINZ1.movement_ok_flag=0;
		}
		else if(PRINZ1.flag_dir==OUT &&(PRINZ1.count!=0)&&PRINZ1.movement_ok_flag){
			if(PRINZ1.M1late_flag && !PRINZ1.M2late_flag) {
				PR_INZ_M1_Move();
			}
			else if(PRINZ1.M2late_flag && !PRINZ1.M1late_flag) {
				PR_INZ_M2_Move();
			}
			else if(!(PRINZ1.M1late_flag && PRINZ1.M2late_flag)) {
				PR_INZ_Move_Both(PRINZ1.MotorSteps[PRINZ1.count]);
				PRINZ1.count--;
			}
			PRINZ1.movement_ok_flag=0;
		}
		if(PRINZ1.count==COUNT_MAX-1) PRINZ1.flag_dir=OUT;
		else if(PRINZ1.count==0 &&PRINZ1.M1_stop_flag&&PRINZ1.M2_stop_flag){
			PRINZ1.type_of_task=END_OF_TASK;
		}
		break;

	case RETURNING://what we do while returning to start
		  PRINZ1.flag_dir=OUT;
		  LCD_Clear(&LCD1);
		  LCD_Print("Return to start",&LCD1);
		  while(!(GPIOC->IDR & M1_start_Pin) || !(GPIOC->IDR & M2_start_Pin)){
			  if(!PRINZ1.M1_start_flag&&!(GPIOC->IDR & M1_start_Pin)) {
				  PR_INZ_M1_Move();
			  }
			  if(!PRINZ1.M2_start_flag&&!(GPIOC->IDR & M2_start_Pin)) {
				  PR_INZ_M2_Move();
			  }

		  }
		  PRINZ1.type_of_task=STANDBY;
		break;

	case END_OF_TASK://what we do if its the end of task
		  PR_INZ_Stop();
		  LCD_Clear(&LCD1);
		  LCD_Set_Cursor(0, 0, &LCD1);
		  LCD_Print("end of process ", &LCD1);
		  PRINZ1.type_of_task=STANDBY;
		break;
	default:
		break;
	}
}


/* @brief: callback for all gpio interrupts*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	switch (GPIO_Pin) {

		case START_BUTTON_Pin: // button on encoder , using only for starting a process
			PRINZ1.type_of_task=FULL_TASK;
			PR_INZ_Start();

			break;

		case STOP_BUTTON_Pin:

			PRINZ1.type_of_task=END_OF_TASK;
			//PR_INZ_Stop();
			break;

		case M1_start_Pin:
			PRINZ1.M1_start_flag=1;
			if((GPIOC->IDR & (M1_start_Pin|M2_start_Pin))&&(PRINZ1.M1_stop_flag&&PRINZ1.M2_stop_flag)){ //GPIO->IDR & M1_start_Pin|M2_start_Pin
				PRINZ1.type_of_task=END_OF_TASK;
				//PR_INZ_Stop();
			}

			break;

		case M1_stop_Pin:

			PRINZ1.M2late_flag=1;
			PRINZ1.M1_stop_flag=1;
			if(PRINZ1.M1_stop_flag&&PRINZ1.M2_stop_flag){
				PRINZ1.M1late_flag=0;
				PRINZ1.M2late_flag=0;
				PRINZ1.flag_dir=OUT;
			}
			break;

		case M2_start_Pin:


			PRINZ1.M2_start_flag=1;
			if((GPIOC->IDR & (M1_start_Pin|M2_start_Pin))&&(PRINZ1.M1_stop_flag&&PRINZ1.M2_stop_flag)){ // HAL_GPIO_ReadPin(GPIOC, M2_start_Pin)&&HAL_GPIO_ReadPin(GPIOC, M1_start_Pin)){ //GPIO->IDR & M1_start_Pin|M2_start_Pin
				PRINZ1.type_of_task=END_OF_TASK;
				//PR_INZ_Stop();
			}
		  	  break;

		case M2_stop_Pin:

			PRINZ1.M1late_flag=1;
			PRINZ1.M2_stop_flag=1;
			if(PRINZ1.M1_stop_flag&&PRINZ1.M2_stop_flag){
				PRINZ1.M1late_flag=0;
				PRINZ1.M2late_flag=0;
				PRINZ1.flag_dir=OUT;

			}
			break;
		default:
			break;
	}

}//this callback checks which button is pressed and then set the flag to activate certain function

//maybe i should add different functions to do if smth is triggered
/*
 * 1 idea - magnets moves at the same time if no one is late , if late flag is active - using only single function DONE
 * 2 idea - add that algorithm for different flags such as msr flag,mov flag etc
 * 3 idea -
 *
 */
