/*
 * config.c
 *
 *  Created on: 05.12.2017
 *      Author: gwarchol
 */

#include "device_config.h"


GPIO_InitTypeDef GPIO_InitStruct;


NVIC_InitTypeDef nvic;

static volatile bool sleep_mode_status = false;


void set_sleep_mode() {
	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT, ENABLE);
	sleep_mode_status = true;
}
void clr_sleep_mode() {
	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT, ENABLE);
	sleep_mode_status = false;
}


void sleep_mode_init()
{


	DBGMCU_Config(DBGMCU_STANDBY, ENABLE);


	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT, DISABLE);

}


void EXTI0_IRQHandler(void)
{

	EXTI_ClearITPendingBit(EXTI_Line0);
	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT, DISABLE);
	printf("EXTI4 \n");
}






void device_init(){


	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_Init(GPIOC, &GPIO_InitStruct);



	    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_Init(GPIOC, &GPIO_InitStruct);

//	    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
//	    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//	    GPIO_Init(GPIOA, &GPIO_InitStruct);

}
void init_nvic(){

	 nvic.NVIC_IRQChannel = TIM2_IRQn;
	 nvic.NVIC_IRQChannelPreemptionPriority = 0;
	 nvic.NVIC_IRQChannelSubPriority = 1;
	 nvic.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&nvic);

	 NVIC_EnableIRQ(TIM2_IRQn);

}





void turnOnOffLed(bool state){

	switch(state){
	case true:
		GPIO_WriteBit(GPIOC, GPIO_Pin_13,Bit_RESET );

		break;
	case false:
		GPIO_WriteBit(GPIOC, GPIO_Pin_13,Bit_SET );
		break;
	}

}

void RCC_Conf(void) {
 	/* Konfiguracja zegara -------------------------------------------------------*/
 	/* Wyzerowanie poprzednich ustawień RCC*/
 	RCC_DeInit();

 	ErrorStatus HSEStartUpStatus;

 	/* Włączenie HSE */
 	RCC_HSEConfig(RCC_HSE_ON);

 	/* Oczekiwanie na gotowość HSE
 	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) {
 	}
 	*/
 	/* Czekaj, aż HSE sie uruchomi lub zostanie przekroczony dozwolony czas */
 	HSEStartUpStatus = RCC_WaitForHSEStartUp();

 	if (HSEStartUpStatus == SUCCESS)
 		{
 	/* Włączenie opóźnień dla FLASH */
 	FLASH_SetLatency(FLASH_Latency_2);
 	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

 	/* HCLK = SYSCLK */
 	RCC_HCLKConfig(RCC_SYSCLK_Div1);

 	/* PCLK2 = HCLK */
 	RCC_PCLK2Config(RCC_HCLK_Div1);

 	/* PCLK1 = HCLK/2 */
 	RCC_PCLK1Config(RCC_HCLK_Div2);

 	/* PLLCLK = 8MHz * 9 = 72 MHz
 	 * w linii Conectivity Line nie ma mozliwosci skorzystania bezposrednio z HSE jako zrodla dla petli PLL
 	 * */
 	RCC_PLLConfig(RCC_PLLSource_HSE_Div1 , RCC_PLLMul_9); // ????

 	/* Włączenie PLL */
 	RCC_PLLCmd(ENABLE);

 	/* Oczekiwanie na gotowość PLL */
 	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
 	}

 	/* Wybór PLL na zegar systemowy */
 	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

 	/* Oczekiwanie na wybór zegara systemowego */
 	while (RCC_GetSYSCLKSource() != 0x08) {
 	}
 		}
 }







