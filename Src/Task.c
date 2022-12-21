/*
 * Task.c
 *
 *  Created on: Nov 2, 2021
 *      Author: dang khoa
 */
#include "Task.h"
#include "stm32f407xx.h"
#include "main.h"
#include "stdio.h"
#include "lcd.h"
#include "dht11.h"
//#include "stm32f4xx_it.c"




//For DHT11 operations
float humidity;
float temp_C;
float temp_F;
uint8_t dht11_data[5];
char buffer[50];
char humid_buffer[20];
char temp_buffer[20];
uint8_t num = 0;
int8_t Response = 0;
uint8_t C_or_F = 1;



//UART_HandleTypeDef huart4;









void Display_Response(void)
{
	sprintf(buffer,"Resp: %d",Response);
	lcd_clear();
	lcd_put_cursor(0,0);
	lcd_write_string(buffer);
}


void Read_from_DHT11(void)
{
	DHT11_Start();

	uint8_t try_time = 0;

	Response = Check_Response();

	while(Response == -1 && try_time < 3)
	{
		DHT11_Start();

		Response = Check_Response();

		try_time++;
	}

	if(try_time < 3)
	{
		for (uint8_t i = 0; i < 5; i++)
		{
			dht11_data[i] = DHT11_Read();
		}
	}
}


void Display_Data_from_DHT11(void)
{

/*	if((dht11_data[0] | dht11_data[1] | dht11_data[2] | dht11_data[3]) != dht11_data[4])
	{
		lcd_clear();
		lcd_put_cursor(0,0);
		lcd_write_string("Corrupted data");
		return;
	}*/

	if(Response != -1)
	{
		humidity = (float)dht11_data[0];//(float)((float)((dht11_data[0]<<8) + dht11_data[1])/(float)10.0);
		temp_C = (float)dht11_data[2];//(float)((float)((dht11_data[2]<<8) + dht11_data[3])/(float)10.0);
		temp_F = temp_C * 1.8;
		lcd_clear();
		lcd_put_cursor(0,0);
		sprintf(humid_buffer,"Humidity: %.1f%%",humidity);
		lcd_write_string(humid_buffer);
		lcd_put_cursor(1,0);
		if(C_or_F) sprintf(temp_buffer,"Temp: %.1fC",temp_C);
		else sprintf(temp_buffer,"Temp: %.1fF",temp_F);
		lcd_write_string(temp_buffer);
	}

	else
	{
		lcd_clear();
		lcd_write_string("In-responsive sensor");
	}
}





void Display_number(void)
{
	char *message = "Curr_num: %d";
	lcd_clear();
	lcd_put_cursor(0,0);

	sprintf(buffer,message,num);
	num = (num+1)%100;
	lcd_write_string(buffer);
}



void Led_blinking1(void)
{
	GPIOD->ODR ^=(1<<12);
	//HAL_UART_Transmit(&huart2, task1,sizeof(task1),1000);
}


void Led_blinking2(void)
{
	GPIOD->ODR ^=(1<<13);
	//HAL_UART_Transmit(&huart2, task2,sizeof(task2),1000);
}

void Led_blinking3(void)
{
	GPIOD->ODR ^= (1<<14);
	//HAL_UART_Transmit(&huart2, task3,sizeof(task3),1000);
}

void Led_blinking4(void)
{
	GPIOD->ODR ^= (1<<15);
	//HAL_UART_Transmit(&huart2, task4,sizeof(task4),1000);
}

