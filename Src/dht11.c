#include "main.h"
#include "Timer.h"



//Pin 3 of GPIOB port will be used for single data line communication
//Clock enable for Port B has been initialized in the main.c file



void Data_line_pin_init(void)
{
	GPIOB->MODER &= ~(3<<6); //Initially set pin 3 as input
	GPIOB->PUPDR &= ~(3<<6);
	//GPIOB->OSPEEDR &= ~(3<<6);
}


void Set_Pin_Output(void)
{
	GPIOB->MODER = (GPIOB->MODER & ~(3<<6))|(1<<6);
	GPIOB->OTYPER &= ~(1<<3); //Set output type as open_drain
		GPIOB->OSPEEDR &= ~(3<<6); //Set output speed to low
}

void Set_Pin_Input(void)
{
	GPIOB->MODER &= ~(3<<6);
	GPIOB->PUPDR &= ~(3<<6);
}



void DHT11_Start(void)
{
	Set_Pin_Output(); //Set the pin as output
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, 0); //Pull the pin low
	HAL_Delay(18); //wait for 18ms
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, 1);
	Delay_micro(20); //wait for 20 microseconds
	Set_Pin_Input();
}


int8_t Check_Response(void)
{
	int8_t Response = 0;

	Delay_micro(40);

	if(!(GPIOB->IDR & (1<<3))){

		Delay_micro(80);

		if(GPIOB->IDR & (1<<3)) Response= 1;

		else Response = -1;

	}

	int count = 0;

	while(GPIOB->IDR & (1<<3)){

		count ++;

	} // wait for the pin to go low

	return Response;
}

uint8_t DHT11_Read(void)
{
	uint8_t i = 0;
	uint8_t j;

	for(j = 0; j < 8; j++)
	{
		while(!(GPIOB->IDR & (1<<3))){}  // wait for the pin to go high

		Delay_micro(40);  // wait for 40 us

		if(!(GPIOB->IDR & (1<<3)))  // if the pin is low
		{
			i&= ~(1<<(7-j)); // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1

		while(GPIOB->IDR & (1<<3)){} // wait for the pin to go low
	}

	return i;
}


