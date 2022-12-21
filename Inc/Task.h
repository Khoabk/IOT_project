/*
 * Task.h
 *
 *  Created on: Nov 2, 2021
 *      Author: dang khoa
 */


#include "main.h"

#ifndef INC_TASK_H_
#define INC_TASK_H_

//UART_HandleTypeDef huart2;



extern float humidity;
extern float temp_C;
extern float temp_F;

extern uint8_t C_or_F;


extern uint8_t temp;
extern uint8_t command[30];
extern uint8_t index_command;
extern uint8_t buffer_flag;
extern char humid_buffer[20];
extern char temp_buffer[20];


void Led_blinking1(void);

void Led_blinking2(void);

void Led_blinking3(void);

void Led_blinking4(void);

void Led_blinking5(void);

void Display_number(void);

void Read_from_DHT11(void);

void Display_Data_from_DHT11(void);

void Display_Response(void);

/*void Send_resp (void);

void Request_parser(void);

void Send_info (void);*/


#endif /* INC_TASK_H_ */
