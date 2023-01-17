/*
 * NVIC_Service.c
 *
 *  Created on: 20 груд. 2022 р.
 *      Author: vovag
 */

#include "NVIC_Service.h"
#include "usart.h"
#include "PR_INZ.h"
#include "ADS1115.h"
#include "stdbool.h"

extern ADS1115_InitTypeDef ADS1115;
extern LCD_InitTypeDef 	LCD1;
extern PR_INZ_InitTypeDef 	PRINZ1;

int	atoi (const char *__nptr);

char 				Rx_data[4],Act_data[3];
uint8_t 			ucUART_1byteflag,ucUART_Number;
bool 				Receive_Callback_Flag=0;
const uint8_t		Tx_fb_move[FeedbackDataLen]=			"Moving start\n",
					Tx_fb_stop[FeedbackDataLen]=			"Moving abort\n",
					Tx_fb_measure[FeedbackDataLen]=			"Measurement \n",
					Tx_fb_SingleShot[FeedbackDataLen]=		"Single move.\n",
					Tx_fb_Error_Handler[FeedbackDataLen]=	"wrong cmd   \n",
					Tx_fb_Returning[FeedbackDataLen]=		"Returning   \n";
const uint8_t		comp_sta[3]="sta",
					comp_stp[3]="stp",
					comp_msr[3]="msr",
					comp_ret[3]="ret";

//private function for just not to copy a lot of the same functions
static inline void UART_Service_stp_config_Private(){
	HAL_UART_Transmit_DMA(&huart2, Tx_fb_stop, FeedbackDataLen);
	PR_INZ_Stop();
	PRINZ1.type_of_task=END_OF_TASK;
	LCD_Set_Cursor(1, 12, &LCD1);
	LCD_Print(Rx_data, &LCD1);
}
static inline void UART_Service_ret_config_Private(){
	HAL_UART_Transmit_DMA(&huart2, Tx_fb_Returning, FeedbackDataLen);
	PRINZ1.type_of_task=RETURNING;
	LCD_Set_Cursor(1, 12, &LCD1);
	LCD_Print(Rx_data, &LCD1);
}

static inline void UART_Service_sta_config_Private(){
	HAL_UART_Transmit_DMA(&huart2, Tx_fb_move, FeedbackDataLen);
	PRINZ1.type_of_task=MOVING_ONLY;
	PRINZ1.flag_dir=ucUART_1byteflag;
	if(PRINZ1.flag_dir) PRINZ1.count=0;
	else PRINZ1.count=COUNT_MAX-1;
	PR_INZ_Start();
	LCD_Set_Cursor(1, 12, &LCD1);
	LCD_Print(Rx_data, &LCD1);
}

static inline void UART_Service_mov_config_Private(){
	HAL_UART_Transmit_DMA(&huart2, Tx_fb_SingleShot, FeedbackDataLen);
	PRINZ1.flag_dir=ucUART_1byteflag;
	PR_INZ_Move_Both(ucUART_Number);
	LCD_Set_Cursor(1, 12, &LCD1);
	LCD_Print(Rx_data, &LCD1);
	PRINZ1.type_of_task=STANDBY;
}

static inline void UART_Service_msr_config_Private(){
	HAL_UART_Transmit_DMA(&huart2, Tx_fb_measure, FeedbackDataLen);
	PRINZ1.type_of_task=MEASUREMENT_ONLY;
}

/* @brief: function just to send error message if user sends smth wrong*/
static inline void UART_Command_Error_Handler_Private(){
	HAL_UART_Transmit_DMA(&huart2,Tx_fb_Error_Handler, FeedbackDataLen);
	LCD_Set_Cursor(1, 12, &LCD1);
	LCD_Print(Rx_data, &LCD1);
}

/* @brief: function for processing all that messages from PC*/
void UART_Service(){
	Receive_Callback_Flag=0;
	memcpy(Act_data,Rx_data+1,sizeof(Act_data));
	ucUART_1byteflag=atoi(&Rx_data[0]);
	ucUART_Number=atoi((char*)Act_data);
	if(ucUART_1byteflag==1 || ucUART_1byteflag==0){
		if(!memcmp(Act_data,comp_sta,sizeof(Act_data))) 		UART_Service_sta_config_Private();
		else if(!memcmp(Act_data,comp_msr,sizeof(Act_data))) 	UART_Service_msr_config_Private();
		else if(ucUART_Number)									UART_Service_mov_config_Private();
		else if(!memcmp(Act_data,comp_stp,sizeof(Act_data)))	UART_Service_stp_config_Private();
		else if(!memcmp(Act_data,comp_ret,sizeof(Act_data)))	UART_Service_ret_config_Private();
		else UART_Command_Error_Handler_Private();
	}
	else if(ucUART_1byteflag==2){
		if(!memcmp(Act_data,comp_msr,sizeof(Act_data))) 	UART_Service_msr_config_Private();
		else UART_Command_Error_Handler_Private();
	}
	else UART_Command_Error_Handler_Private();

}
/* @brief: callback function in IRQ*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	Receive_Callback_Flag=1;
	HAL_UART_Receive_DMA(&huart2,(uint8_t*)Rx_data,sizeof(Rx_data));
}
