/*
 * dht11.h
 *
 *  Created on: Dec 11, 2022
 *      Author: dang khoa
 */

#include "stdint.h"

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

void Data_line_pin_init(void);

void Set_Pin_Output(void);

void Set_Pin_Iutput(void);

void DHT11_Start(void);

int8_t Check_Response(void);

uint8_t DHT11_Read(void);

#endif /* INC_DHT11_H_ */
