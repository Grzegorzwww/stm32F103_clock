/*
 * touch_screen.h
 *
 *  Created on: 26.02.2018
 *      Author: gwarchol
 */

#include "touch_screen.h"

SPI_InitTypeDef SPI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

uint8_t SPIy_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                      0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                      0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                                      0x1D, 0x1E, 0x1F, 0x20};
uint8_t SPIz_Buffer_Tx[BufferSize] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                                      0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
                                      0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,
                                      0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
                                      0x6D, 0x6E, 0x6F, 0x70};




void init_spi(){


	//	  RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_APB2PeriphClockCmd(SPI_2_CLK  | SPI_2_GPIO_CLK, ENABLE);

	/* Configure SPIy pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = SPI_2_PIN_SCK | SPI_2_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	/* Configure SCK and MOSI pins as Input Floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_2_GPIO, &GPIO_InitStructure);



	/* SPIy Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_2, &SPI_InitStructure);


	SPI_Cmd(SPI_2, ENABLE);


//	  SPI_I2S_ITConfig(SPI_2, SPI_I2S_IT_TXE, ENABLE);
//	  /* Enable SPI_SLAVE RXNE interrupt */
	  SPI_I2S_ITConfig(SPI_2, SPI_I2S_IT_RXNE, ENABLE);




	  NVIC_InitTypeDef NVIC_InitStructure;

	  /* 1 bit for pre-emption priority, 3 bits for subpriority */
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	  /* Configure and enable SPI_MASTER interrupt -------------------------------*/
	  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

}





