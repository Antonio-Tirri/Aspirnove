#include "pwm.h"
#include "board.h"
#include "tim.h"








int PWM_Init(void)
{

	// PWM_FAN1
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
	
	// PWM_FAN2
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);

	// PWM_H2O
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);

	// PWM_BATT
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_4);
	return 0;
}


int PWM_Set(uint8_t channel, uint16_t value)
{	
	switch(channel)
	{
		case (TIMER2 + TIM_CHANNEL_1):
			TIM2->CCR1 = value;
		break;

		case (TIMER2 + TIM_CHANNEL_2):
			TIM2->CCR2 = value;
		break;

		case (TIMER2 + TIM_CHANNEL_3):
			TIM2->CCR3 = value;
		break;

		case (TIMER2 + TIM_CHANNEL_4):
			TIM2->CCR4 = value;
		break;
		
		default:
			return -1;
	}
	
	return 0;
}



uint16_t PWM_GetMaxValue(uint8_t channel)
{
	switch (channel & 0xF0)
	{
		default:
			return 0;

		case (TIMER2):
			return htim2.Init.Period + 1;

	}

}


int PWM_SetPercent(uint8_t channel, uint16_t percent_value)
{	
	uint16_t value;
	uint32_t period;
	
	if (percent_value > 100)
		return -1;
	
	period = PWM_GetMaxValue(channel);
	
	value = (uint16_t)((uint32_t)(period * (uint32_t)percent_value) / 100L);
	
	return PWM_Set(channel, value);
}





uint8_t PID_H2O_Poll(int16_t error)
{
	// Costanti PID
	#define H2O_KP_MIN      0.01
	#define H2O_KI_MIN      0.007

	static float h2oCmdI = 0;
	float h2oCmdP, h2oPID;

	if (error < 0)
		h2oCmdI = 0;

	h2oCmdP = (float)error * H2O_KP_MIN;
	h2oCmdI += ((float)error * H2O_KI_MIN);
	
    // Somma comandi
	h2oPID = h2oCmdP + h2oCmdI;

    // Verifica validità valore
    if (h2oPID > PWM_GetMaxValue(PWM_H2O))
    	h2oPID = PWM_GetMaxValue(PWM_H2O);
    else
        if (h2oPID < 0)
        	h2oPID = 0;
    
    // Comando PWM
    PWM_Set(PWM_H2O, h2oPID);
    
    
    h2oPID = (h2oPID * 100) / PWM_GetMaxValue(PWM_H2O);

    return (uint8_t)h2oPID;
}


uint8_t PID_FAN_Poll(uint8_t fan, uint16_t cmd_base, int16_t error)
{
	// Costanti PID
	#define FAN_KP_MIN      1
	#define FAN_KI_MIN      0.0001

	static float fanCmdI[2];
	float fanCmdP, fanPID;

	if (error < 0)
		fanCmdI[fan] = 0;


	fanCmdP = (float)error * FAN_KP_MIN;
	fanCmdI[fan] += ((float)error * FAN_KI_MIN);



    // Somma comandi
	fanPID = fanCmdP + fanCmdI[fan];

    // Verifica validità valore
    if (fanPID > PWM_GetMaxValue(PWM_FAN1))
    	fanPID = PWM_GetMaxValue(PWM_FAN1);
    else
        if (fanPID < 0)
        	fanPID = 0;

    fanPID = (fanPID * 100L) / PWM_GetMaxValue(PWM_FAN1);	// PWM_FAN1 e PWM_FAN2 hanno lo stesso periodo

    fanPID += cmd_base;

    if (fanPID > 100)
    	fanPID = 100;

    return (uint8_t)(fanPID);
}
	
