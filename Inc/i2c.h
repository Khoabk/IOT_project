/*
 * i2c.h
 *
 *  Created on: Sep 12, 2022
 *      Author: dang khoa
 */

#include "main.h"

#ifndef INC_I2C_H_
#define INC_I2C_H_

extern uint8_t address;

void I2C_init (void);

void I2C_Send (uint8_t address,uint8_t * mess, uint32_t len);


#endif /* INC_I2C_H_ */
