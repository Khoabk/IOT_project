/*
 * Timer.c
 *
 *  Created on: Nov 2, 2021
 *      Author: dang khoa
 */

#include "main.h"
#include "stdint.h"


void Timer_init(void)
{
	MX_TIM2_Init();

	MX_TIM9_Init();
}

/*static uint16_t period_time = 0;


void Reset_timer(void)
{
	period_time = 0;
}


void Update_timer(void)
{
	period_time ++;
}
*/

void Delay_micro(uint16_t delay_time)
{
	//Reset_timer();

	TIM9->CNT = 0;

	GPIOD->ODR |= (1<<13);

	while(TIM9->CNT < delay_time){}

	GPIOD->ODR &= ~(1<<13);
}


void Delay_mili(uint16_t delay_time)
{
	TIM9->CNT = 0;

	uint16_t mili_time = 0;

	while(mili_time < delay_time)
	{
		if(TIM9->CNT >= 1000)
		{
			TIM9->CNT = 0;

			mili_time++;
		}
	}


}

