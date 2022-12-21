/*
 * lcd.c
 *
 *  Created on: Sep 12, 2022
 *      Author: dang khoa
 */


#include "i2c.h"
#include "main.h"




void lcd_write_cmd(char cmd){
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C; //en=1, rs=0
	data_t[1] = data_u|0x08; //en=0, rs=0
	data_t[2] = data_l|0x0C; //en=1, rs=0
	data_t[3] = data_u|0x08; //en=0, rs=0
	I2C_Send(address,data_t, 4);
}





void lcd_shift(uint8_t RL, uint8_t SC){
	// R/L =0: shift to the left
	// R/L =1: shift to the right
	// S/C =0: curso move
	// S/C =1: display shift
	uint8_t cmd = 0x10;
	cmd = cmd | (RL << 2) | (SC << 3);
	lcd_write_cmd(cmd);
}





void lcd_write_data(char data){
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D; //en=1, rs=1
	data_t[1] = data_u|0x09; //en=0, rs=1
	data_t[2] = data_l|0x0D; //en=1, rs=1
	data_t[3] = data_u|0x09; //en=0, rs=1
	I2C_Send(address,data_t, 4);
}


void lcd_init(void){

	//4bit initialization
	HAL_Delay(40);
	lcd_write_cmd(0x30);
	HAL_Delay(5);
	lcd_write_cmd(0x30);
	HAL_Delay(1);
	lcd_write_cmd(0x30);
	HAL_Delay(10);
	lcd_write_cmd(0x20);
	HAL_Delay(10);

	//display initialization
	lcd_write_cmd(0x20);
	HAL_Delay(10);
	lcd_write_cmd(0x28);
	HAL_Delay(10);
	lcd_write_cmd(0x14);
	HAL_Delay(10);
	lcd_write_cmd(0x0f);
	HAL_Delay(10);
	lcd_write_cmd(0x01);
	HAL_Delay(10);
	lcd_write_cmd(0x06);
}

void lcd_clear(void){
	lcd_write_cmd(0x01);
	HAL_Delay(1);
}

void lcd_return_home(void){
	lcd_write_cmd(0x02);
	HAL_Delay(2);
}


void lcd_write_string(char *str){
    while(*str) lcd_write_data(*str++);
}



void lcd_cursor_display_mode (uint8_t on_off,uint8_t display, uint8_t blink){

	//on_of: 1 => cursor is displayed and vice versa
	//display: 1 => display data and vice versa
	//blink: 1 => cursor blinks and vice versa

	uint8_t cmd = (0x00 ^ ((on_off & (0x01))<<1|(blink & (0x01))|(display & 0x01)<<2))|0x08;

	lcd_write_cmd(cmd);

	HAL_Delay(1);
}



void lcd_put_cursor(uint8_t row, uint8_t col){

	//row is 0 or 1
	//col is between 0 and 15

	switch (row)
	{
	case 0: col |= 0x80;
			break;
	case 1: col |= 0xc0;
			break;
	}

	lcd_write_cmd(col);

	HAL_Delay(1);
}

