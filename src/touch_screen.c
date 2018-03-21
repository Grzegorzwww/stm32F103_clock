/*
 * touch_screen.h
 *
 *  Created on: 26.02.2018
 *      Author: gwarchol
 */

#include "touch_screen.h"

SPI_InitTypeDef SPI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
DMA_InitTypeDef dmaStructure;

volatile unsigned char touch_screeen_data_in[TOUCH_SCREEN_BUFFER_SIZE];

volatile unsigned char touch_screeen_data[TOUCH_SCREEN_BUFFER_SIZE];

volatile unsigned char touch_screeen_data_out[TOUCH_SCREEN_BUFFER_SIZE] = {
		TOUCH_SCREEN_GET_Z_VALUE_COMMAND, TOUCH_SCREEN_DUMMY_WORLD,
		TOUCH_SCREEN_GET_X_VALUE_COMMAND, TOUCH_SCREEN_DUMMY_WORLD,
		TOUCH_SCREEN_GET_Y_VALUE_COMMAND, TOUCH_SCREEN_DUMMY_WORLD
};


DMA_InitTypeDef DMA_InitStructure;


void init_spi_2(){

	//	  RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2 , ENABLE);

	/* Configure SPIy pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = SPI_2_PIN_SCK | SPI_2_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_2_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_2_PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(SPI_2_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_2_PIN_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_2_GPIO, &GPIO_InitStructure);

	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	//	init_spi_2_dma();
	SPI_Cmd(SPI2, ENABLE);
	SPI_I2S_ITConfig(SPI_2, SPI_I2S_IT_RXNE, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	touch_screen_cs_high();

}

void init_spi_2_dma(){


	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2 , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	/* Configure SPIy pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = SPI_2_PIN_SCK | SPI_2_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_2_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_2_PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(SPI_2_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_2_PIN_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_2_GPIO, &GPIO_InitStructure);



    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // TX
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
    // RX
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);

    touch_screen_cs_high();


}

inline static void spi2_dmaReceive8(u8 *data, u32 n) {
	DMA_StructInit(&dmaStructure);
    dmaStructure.DMA_MemoryBaseAddr = (u32) data;
    dmaStructure.DMA_BufferSize     = n;

    dmaStructure.DMA_Mode               = DMA_Mode_Normal;
    dmaStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    dmaStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    dmaStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    dmaStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;


    DMA_Init(DMA1_Channel5, &dmaStructure);
    DMA_Cmd(DMA1_Channel5, ENABLE);

}

inline static void spi2_dmaSend8(u8 *data, u32 n) {
	DMA_StructInit(&dmaStructure);
	dmaStructure.DMA_PeripheralBaseAddr = (u32) &(SPI2->DR);
	dmaStructure.DMA_Priority           = DMA_Priority_Medium;

	dmaStructure.DMA_MemoryBaseAddr = (u32) data;
	dmaStructure.DMA_BufferSize     = n;

	dmaStructure.DMA_Mode               = DMA_Mode_Normal;
	dmaStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	dmaStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
	dmaStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	dmaStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	DMA_Init(DMA1_Channel4, &dmaStructure);
	DMA_Cmd(DMA1_Channel4, ENABLE);
}


void touch_screen_transmit_dma_data_set(){
	unsigned char cmd = TOUCH_SCREEN_GET_Z_VALUE_COMMAND;
	u8 dummy = 0x00;
	touch_screen_cs_low();
	spi2_dmaSend8(&cmd, 1);
	spi2_dmaSend8(&dummy, 1);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY) == SET);
	touch_screen_cs_high();
}

void touch_screen_receive_dma_data_set(){
	;
	u8 data = 0;


	spi2_dmaReceive8(&data, 1);

	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY) == SET);

	printf("spi 2 data = %d\n", data);

}



//void DMA2_Channel1_IRQHandler(void) {
//	printf("+");
//	if (DMA_GetITStatus(DMA2_IT_TC1) == SET) {
////		DMA_Cmd(DMA2_Channel1, DISABLE);
//
//		DMA_ClearITPendingBit(DMA2_IT_TC1);
//	}
//}


void DMA1_Channel4_IRQHandler(void) {

	if (DMA_GetITStatus(DMA1_IT_TC4) == SET) {
		DMA_Cmd(DMA1_Channel4, DISABLE);
		printf("!");
		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}
}

void DMA1_Channel5_IRQHandler(void) {
	if (DMA_GetITStatus(DMA1_IT_TC5) == SET) {
		DMA_Cmd(DMA1_Channel5, DISABLE);
		printf("?");
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}
}



void send_to_touch_screen(unsigned short command){

}

void init_touch_screen(){

	init_spi_2();


}

volatile int save_index = 0;
volatile unsigned char z_axis = 0;
volatile unsigned char x_axis = 0;
volatile unsigned char y_axis = 0;
touch_screen_state_t touch_screen_state = ASK_TO_READ_Z_AXIS;

void analize_data_from_touch_screen(bool on_off, unsigned char * str){


	switch(touch_screen_state){
	case ASK_TO_READ_Z_AXIS :
		touch_screen_send_command(0xB3);
		touch_screen_send_command(0x00);
		touch_screen_state = WAIT_TO_READ_Z_AXIS;
		break;
	case ASK_TO_READ_X_AXIS :
		touch_screen_send_command(0x90);
		touch_screen_send_command(0x00);
		touch_screen_state = WAIT_TO_READ_X_AXIS;
		break;
	case ASK_TO_READ_Y_AXIS :
		touch_screen_send_command(0xD0);
		touch_screen_send_command(0x00);
		touch_screen_state = WAIT_TO_READ_Y_AXIS;
		break;
	}


		sprintf(str, "z = %d x = %d, y = %d\0", z_axis, x_axis, y_axis);

}






void SPI2_IRQHandler(void) {
	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET) {
		//Send from buffer
		//printf("przerwanie SPI TXE\n");
		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
	}

	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET) {

		unsigned char dummy_read;
//			printf("save_index = %d - ", save_index++);
//		unsigned char data_8  = (unsigned char)SPI_I2S_ReceiveData(SPI2);
			//printf("3\n");
			if(touch_screen_state == WAIT_TO_READ_Z_AXIS){
				z_axis =  (unsigned char)SPI_I2S_ReceiveData(SPI2);
				touch_screen_state = ASK_TO_READ_X_AXIS;
				//printf("z_axis = %d ",z_axis );
			}else if (touch_screen_state == WAIT_TO_READ_X_AXIS){
				x_axis =  (unsigned char)SPI_I2S_ReceiveData(SPI2);
				touch_screen_state = ASK_TO_READ_Y_AXIS;
				//printf("x_axis = %d ",x_axis );

			}else if (touch_screen_state == WAIT_TO_READ_Y_AXIS){
				y_axis =  (unsigned char)SPI_I2S_ReceiveData(SPI2);
				touch_screen_state = ASK_TO_READ_Z_AXIS;
				//printf("y_axis = %d\n",y_axis );
			}
			else{
				dummy_read = (unsigned char)SPI_I2S_ReceiveData(SPI2);
				//printf("dummy_read  = %d",dummy_read  );
			}


//		printf("data  = %d ", data_8);

//		printf("data_8  = %d ", data_8);

//		 touch_screeen_data[save_index] = (data << 8);
//			if(save_index > 5 ) {save_index = 0;}

		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
	}
}

void touch_screen_send_command(unsigned short command){

	touch_screen_cs_low();
	SPI_I2S_SendData(SPI2,command);
	touch_screen_cs_high();
}



void touch_screen_cs_low(void){
	GPIO_WriteBit(SPI_2, SPI_2_PIN_NSS,Bit_RESET );
}

void touch_screen_cs_high(void){
	GPIO_WriteBit(SPI_2, SPI_2_PIN_NSS,Bit_SET );
}







