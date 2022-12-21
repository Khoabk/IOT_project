/*
 * Timer.h
 *
 *  Created on: Nov 2, 2021
 *      Author: dang khoa
 */

#include "stdint.h"

#ifndef INC_TIMER_H_
#define INC_TIMER_H_


void Timer_init(void);

//void Reset_timer(void);

//void Update_timer(void);

void Delay_micro(uint16_t delay_time);

void Delay_mili(uint16_t delay_time);

#endif /* INC_TIMER_H_ */
