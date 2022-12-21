/*
 * i2c.c
 *
 *  Created on: Sep 12, 2022
 *      Author: dang khoa
 */

#include "main.h"

uint8_t address = 0x7E;

void I2C_Write (uint8_t data)
{
//	while(!(I2C1->SR1 & (1<<7))); //Wait for TxE bit to set
	I2C1->DR = data;
	while(!(I2C1->SR1 & (1<<7))); //Wait for BTF bit to set
}

void I2C_Address (uint8_t address)
{
	I2C1->DR = address;
	while(!(I2C1->SR1 & (1<<1)));
	I2C1->SR1;
	I2C1->SR2;
}

void I2C_Start (void)
{
	I2C1->CR1 |= (1<<8);//Generate start
	while(!(I2C1->SR1 &(1<<0))); //Wait for SB bit to be set
}


void I2C_Stop (void)
{
	I2C1->CR1 |= (1<<9);
}

void I2C_init (void)
{
	 I2C1->CR1 |= (1<<15); //Reset the I2C
	 I2C1->CR1 &= ~(1<<15); //Normal operation
	 I2C1->CR2 |= (2<<0); //Set the I2C clock 2MHZ
	 I2C1->CCR = (10<<0);
	 I2C1->TRISE = 3;
	 I2C1->CR1 |= (1<<0);//Enable peripheral
	 I2C1->CR1 |= (1<<10); //Enable the ACK
}


void I2C_Send (uint8_t address, uint8_t * mess, uint32_t len)
{
     I2C_Start();

     I2C_Address(address);

     for(int i = 0; i < len; i++)
     {
    	 I2C_Write(mess[i]);
     }

     I2C_Stop();
}







