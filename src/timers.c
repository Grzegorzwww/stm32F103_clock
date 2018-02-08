/*
 * timers.c
 *
 *  Created on: 05.12.2017
 *      Author: gwarchol
 */


#include "timers.h"


 TIM_TimeBaseInitTypeDef tim;




void init_timers(){


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 72;
	tim.TIM_Period = 1000 - 1;
	//tim.TIM_RepetitionCounter = 0;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &tim);


	TIM_ClearFlag( TIM2, TIM_FLAG_Update );
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	init_nvic();


	TIM_Cmd(TIM2, ENABLE);






}



