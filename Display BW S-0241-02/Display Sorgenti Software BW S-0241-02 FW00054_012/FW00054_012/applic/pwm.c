#include "pwm.h"
#include "board.h"
#include "tim.h"



int PWM_Init(void)
{

	// PWM_BACKLIGHT
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	
	// PWM_BUZZER
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_4);

	return 0;
}


int PWM_Set(uint8_t channel, uint16_t value)
{	
	switch(channel)
	{
		case (TIMER1 + TIM_CHANNEL_1):
			TIM1->CCR1 = value;	
		break;
		

		case (TIMER3 + TIM_CHANNEL_4):
			TIM3->CCR4 = value;
		break;

		
		default:
			return -1;
	}
	
	return 0;
}



int PWM_SetPercent(uint8_t channel, uint16_t percent_value)
{	
	uint16_t value;
	uint32_t period;
	
	if (percent_value > 100)
		return -1;
	
	switch (channel & 0xF0)
	{
		default:
		case (TIMER1):
			period = (uint32_t)htim1.Init.Period + 1;
		break;
		
		case (TIMER3):
			period = (uint32_t)htim3.Init.Period + 1;
		break;
	}		
	
	value = (uint16_t)((uint32_t)(period * (uint32_t)percent_value) / 100L);
	
	return PWM_Set(channel, value);
}

	

