/*
 * uart_interrupt.h
 *
 *  Created on: 12.12.2017
 *      Author: gwarchol
 */

#ifndef UART_INTERRUPT_H_
#define UART_INTERRUPT_H_
#include "config.h"
#include "string.h"
#include "buffors.h"




typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

#define countof(a)   (sizeof(a) / sizeof(*(a)))


#define USARTy                   USART1
#define USARTy_GPIO              GPIOA
#define USARTy_CLK               RCC_APB2Periph_USART1
#define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USARTy_RxPin             GPIO_Pin_10
#define USARTy_TxPin             GPIO_Pin_9
#define USARTy_IRQn              USART1_IRQn
#define USARTy_IRQHandler        USART1_IRQHandler



USART_InitTypeDef USART_InitStructure;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

void uart_interrup_init();

void uart_send_data(unsigned char *data, unsigned int data_size);


void init_uart2_polling();

void uart2_send_char(char c);


void analizeIncomingData();



#endif /* UART_INTERRUPT_H_ */
