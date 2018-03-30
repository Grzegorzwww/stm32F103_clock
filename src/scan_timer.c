#include "scan_timer.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "misc.h"


//Podstawa czasu to 50 Khz -> 0.02 ms
//czyli 1 ms = 0.02 * 50 -> czyli CC... = 50 dla 1 ms
//dziekuje za uwage

__IO uint16_t CC4 = 50 ;// 1ms to 50
__IO uint16_t CC3 = 500;// 1ms to 50 -> 500 to 10 ms id...
__IO uint16_t CC2 = 5000; // 100 ms
__IO uint16_t CC1 = 50000 ; //1s


volatile bool _1_ms_flag  =false;
volatile bool _10_ms_flag = false;
volatile bool _100_ms_flag = false;
volatile bool _1_s_flag = false;



bool getTimerChannelState(timer_channel_t channel){
	switch(channel){
	case TIMER_1ms:
		return _1_ms_flag;
		break;
	case TIMER_10ms:
		return _10_ms_flag;
		break;
	case TIMER_100ms:
		return _100_ms_flag;
		break;
	case TIMER_1s:
		return _1_s_flag;
		break;
	default :
		return false;
	}
}

void setTimerChannelState(timer_channel_t channel, bool state){
	switch(channel){
	case TIMER_1ms:
		 _1_ms_flag = state;
		break;
	case TIMER_10ms:
		_10_ms_flag = state;
		break;
	case TIMER_100ms:
		_100_ms_flag = state;
		break;
	case TIMER_1s:
		_1_s_flag = state;
		break;
	default :
		;
	}
}



void init_timer(){

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// Wlacz taktowanie GPIOB i TIM1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	// Ustawienia ukladu podstawy czasu
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 1440;  //fclk = 72MHz/1440 = 50kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);



	// Konfiguracja kanalu 1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CC1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

	// Konfiguracja kanalu 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CC2;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);

	// Konfiguracja kanalu 3
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CC3;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);

	// Konfiguracja kanalu 4
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CC4;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Disable);


	// Wlaczenie przerwan od kanalow
	TIM_ITConfig(TIM1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
	// Wlaczenie timera
	TIM_Cmd(TIM1, ENABLE);



	NVIC_InitTypeDef NVIC_InitStructure;

	// Wlacz przerwanie od TIM1
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


void init_signle_shot_timer(){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


	TIM_TimeBaseInitTypeDef tim;
	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 3;
	tim.TIM_Period = 59;


	//tim.TIM_RepetitionCounter = 0;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &tim);


//	TIM_ClearFlag( TIM3, TIM_FLAG_Update );
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);





	NVIC_InitTypeDef nvic;

	 nvic.NVIC_IRQChannel = TIM3_IRQn;
	 nvic.NVIC_IRQChannelPreemptionPriority = 0;
	 nvic.NVIC_IRQChannelSubPriority = 0;
	 nvic.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&nvic);

	 NVIC_EnableIRQ(TIM3_IRQn);



	TIM_Cmd(TIM3, ENABLE);

}

volatile static bool x = false;

void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){

//		switch(x){
//			case true :
//				GPIO_WriteBit(GPIOB, GPIO_Pin_6,Bit_SET);
////			  	turnOnOffLed(false);
//				x = false;
//				break;
//			case false :
//				GPIO_WriteBit(GPIOB, GPIO_Pin_6,Bit_RESET);
////			  	turnOnOffLed(true);
//				x = true;
//				break;
//
//			}

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


void TIM1_CC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		_1_s_flag = true;
		TIM_SetCompare1(TIM1, TIM_GetCapture1(TIM1) + CC1);
	}
	else if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
		_100_ms_flag = true;
		TIM_SetCompare2(TIM1, TIM_GetCapture2(TIM1) + CC2);
	}
	else if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
		_10_ms_flag = true;
		TIM_SetCompare3(TIM1, TIM_GetCapture3(TIM1) + CC3);
	}
	else if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
		_1_ms_flag = true;
		TIM_SetCompare4(TIM1, TIM_GetCapture4(TIM1) + CC4);
	}
}
