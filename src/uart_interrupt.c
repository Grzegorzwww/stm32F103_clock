/*
 * uart_interrupt.c
 *
 *  Created on: 12.12.2017
 *      Author: gwarchol
 */
#include "uart_interrupt.h"

volatile data_buffor_t _TX_USART_BUFFOR;
volatile data_buffor_t _RX_USART_BUFFOR;

data_buffor_t *get_usart_tx_buffor_ptr()
{

	return &_TX_USART_BUFFOR;
}



void uart_interrup_init(){


	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();

	initBuffor(&_TX_USART_BUFFOR);
	initBuffor(&_RX_USART_BUFFOR);


	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTy, &USART_InitStructure);

	//USARTy->BRR = 36000000U/115200U;
	USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);
	USART_Cmd(USARTy, ENABLE);

}


void RCC_Configuration(void)
{
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE);
}


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USARTy Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

	/* Configure USARTy Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

}


void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

unsigned char temp_buff[100];
unsigned char temp_buff_counter = 0;
unsigned char temp_buff_size = 0;

void uart_send_data(unsigned char *data, unsigned int data_size){

	if(data_size > 0){
		int i;
		for(i = 0; i < data_size; i++){
			addToBuffor(&_TX_USART_BUFFOR, data[i]);
		}
		USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);
	}
}



void USARTy_IRQHandler(void)
{
  if(USART_GetITStatus(USARTy, USART_IT_RXNE) != RESET)
  {
	  //analizeDataFromMP3(USART_ReceiveData(USARTy));

	  addToBuffor(&_RX_USART_BUFFOR, USART_ReceiveData(USARTy));
      USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);

  	//USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);
   }

  if(USART_GetITStatus(USARTy, USART_IT_TXE) != RESET)
  {
    /* Write one byte to the transmit data register */
	volatile unsigned char data_byte = 0x00;
	if(getFromBuffor(&_TX_USART_BUFFOR, &data_byte))
		USART_SendData(USARTy, data_byte);

    if(isBufforEmpty(&_TX_USART_BUFFOR))
    {
      /* Disable the USARTy Transmit interrupt */
      USART_ITConfig(USARTy, USART_IT_TXE, DISABLE);
    }
  }
}


void init_uart2_polling(){
	 GPIO_InitTypeDef gpio;
	 USART_InitTypeDef uart;

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	 GPIO_StructInit(&gpio);
	 gpio.GPIO_Pin = GPIO_Pin_2;
	 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_Init(GPIOA, &gpio);

	 gpio.GPIO_Pin = GPIO_Pin_3;
	 gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA, &gpio);

	 USART_StructInit(&uart);
	 uart.USART_BaudRate = 115200;
	 uart.USART_WordLength = USART_WordLength_8b;
	 uart.USART_StopBits = USART_StopBits_1;
	 uart.USART_Parity = USART_Parity_No;


	 USART_Init(USART2, &uart);

	 USART_Cmd(USART2, ENABLE);



}

void uart2_send_char(char c)
{
 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
 USART_SendData(USART2, c);
}


void analizeIncomingData()
{
	unsigned char temp_byte;
	if(getFromBuffor(&_RX_USART_BUFFOR, &temp_byte) == true){

	analizeDataFromMP3(temp_byte);


	//printf("data = %x \n", temp_byte);

//		if(temp_byte == 0xAA){
//			printf("AA\n");
//		}else if(temp_byte == 0xBB){
//			printf("BB\n");
//		}

	}

}




