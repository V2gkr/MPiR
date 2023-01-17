/*
 * NVIC_Service.h
 *
 *  Created on: 20 груд. 2022 р.
 *      Author: vovag
 */

#ifndef SRC_NVIC_SERVICE_H_
#define SRC_NVIC_SERVICE_H_

#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#define FeedbackDataLen 14

extern char    Rx_data[4];
extern bool    Receive_Callback_Flag;
extern uint8_t ucUART_1byteflag;

/* @brief: function for processing all that messages from PC*/
void UART_Service();
#endif /* SRC_NVIC_SERVICE_H_ */
