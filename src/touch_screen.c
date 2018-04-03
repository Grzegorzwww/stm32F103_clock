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


int counting_period_filter_index = 0;

void set_pushed_filter() {
	counting_period_filter_index = 5;
}
inline int check_pushed_filter(){
	if(counting_period_filter_index < 0)
		counting_period_filter_index = 0;
	return counting_period_filter_index;
}
inline void actualice_pushed_filter(){
	if((counting_period_filter_index) > 0) counting_period_filter_index-- ;

}

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
touch_screen_state_t touch_screen_state = ASK_TO_READ_Z_AXIS;
touch_data_t touch_data;


void analize_data_from_touch_screen(bool on_off){


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



	//		sprintf(str, "z = %d x = %d, y = %d\0", z_axis, x_axis, y_axis);

}

void SPI2_IRQHandler(void) {
	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET) {
		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
	}

	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET) {

		unsigned char dummy_read;
		if(touch_screen_state == WAIT_TO_READ_Z_AXIS){
			touch_data.z_axis = (unsigned char)SPI_I2S_ReceiveData(SPI2);
			touch_screen_state = ASK_TO_READ_X_AXIS;

			if(touch_data.z_axis > 0){
				touch_screen_state = ASK_TO_READ_X_AXIS;
			}else{
				touch_screen_state = ASK_TO_READ_Z_AXIS;
				touch_data.z_axis = 0;
				touch_data.x_axis = 0;
				touch_data.y_axis = 0;
			}

		}else if (touch_screen_state == WAIT_TO_READ_X_AXIS){
			touch_data.x_axis = (unsigned char)SPI_I2S_ReceiveData(SPI2);
			touch_screen_state = ASK_TO_READ_Y_AXIS;


		}else if (touch_screen_state == WAIT_TO_READ_Y_AXIS){
			touch_data.y_axis = (unsigned char)SPI_I2S_ReceiveData(SPI2);
			touch_screen_state = ASK_TO_READ_Z_AXIS;
		}
		else{
			dummy_read = (unsigned char)SPI_I2S_ReceiveData(SPI2);
		}
		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
	}
}


void control_touch_buttons()
{
	//printf("z = %d, x = %d, y = %d\n",touch_data.z_axis,  touch_data.x_axis, touch_data.y_axis);

	if(check_pushed_filter() == 0){

		if(touch_data.z_axis > TOUCH_SCREEN_PUSH_SENS /*&& touch_data.x_axis > 0 && touch_data.y_axis > 0*/){

			//printf("%d, %d\n", touch_data.x_axis, touch_data.y_axis);
			run_command_parameter(103, 125 ,100, 125,  setMenuState, ZEGAR_MENU);
			run_command_parameter( 70, 100,100, 125, setMenuState, BUDZIK_MENU);
			run_command_parameter( 44, 70,100, 125, setMenuState, USTAWIENIA_MENU);
			run_command_parameter( 0, 44, 100, 125, setMenuState, INNE_MENU);


//			if(((touch_data.y_axis > 103) && (touch_data.y_axis < 125)) && ((touch_data.x_axis > 100) && (touch_data.x_axis < 125))){
//
//				setMenuState(ZEGAR_MENU);
//				set_pushed_filter();
//			}
//			if(((touch_data.y_axis > 103) && (touch_data.y_axis < 125)) && ((touch_data.x_axis > 70)&& (touch_data.x_axis < 100))){
//				setMenuState(BUDZIK_MENU);
//				set_pushed_filter();
//			}
//			if(((touch_data.y_axis > 103) && (touch_data.y_axis < 125)) && ((touch_data.x_axis > 44)&& (touch_data.x_axis < 70))){
//				setMenuState(USTAWIENIA_MENU);
//				set_pushed_filter();
//			}
//			if(((touch_data.y_axis > 103) && (touch_data.y_axis < 125)) && ((touch_data.x_axis > 0)&& (touch_data.x_axis < 44))){
//				setMenuState(INNE_MENU);
//				set_pushed_filter();
//			}



			if(get_menu_state() == USTAWIENIA_MENU){

				if(((touch_data.y_axis > 0) && (touch_data.y_axis < 24)) && ((touch_data.x_axis > 0)&& (touch_data.x_axis < 86)) ){
					increment_set_clk_state();
					set_pushed_filter();
				}
//
				if(((touch_data.y_axis > 37) && (touch_data.y_axis < 64)) && ((touch_data.x_axis > 0)&& (touch_data.x_axis < 86)) ){
					increment_set_date_state();
					set_pushed_filter();
				}

				if(((touch_data.y_axis > 70) && (touch_data.y_axis < 93)) && ((touch_data.x_axis > 82)&& (touch_data.x_axis < 116))){
					on_set_up();
					set_pushed_filter();
				}

				if(((touch_data.y_axis > 70) && (touch_data.y_axis < 93)) && ((touch_data.x_axis > 28)&& (touch_data.x_axis < 68))){
					on_set_down();
					set_pushed_filter();
				}


			}
			if(get_menu_state() == BUDZIK_MENU){


				if(((touch_data.y_axis > 0) && (touch_data.y_axis < 24)) && ((touch_data.x_axis > 0)&& (touch_data.x_axis < 86)) ){
					//								increment_set_clk_state();
					//	TODO:
					set_pushed_filter();
				}


				if(((touch_data.y_axis > 70) && (touch_data.y_axis < 93)) && ((touch_data.x_axis > 82)&& (touch_data.x_axis < 116))){
					//TODO:
					set_pushed_filter();
				}

				if(((touch_data.y_axis > 70) && (touch_data.y_axis < 93)) && ((touch_data.x_axis > 28)&& (touch_data.x_axis < 68))){
					//TODO:
					set_pushed_filter();
				}


			}
		}
	}else{
		actualice_pushed_filter();
	}

}



void run_command(uint8_t x_min_limit, uint8_t x_max_limit, uint8_t y_min_limit, uint8_t y_max_limit, void (*fun)(void)){

	if(((touch_data.y_axis > y_min_limit) && (touch_data.y_axis < y_max_limit)) && ((touch_data.x_axis > x_min_limit) && (touch_data.x_axis < x_max_limit))){
			fun();
			set_pushed_filter();
		}

}

void run_command_parameter(uint8_t x_min_limit, uint8_t x_max_limit, uint8_t y_min_limit, uint8_t y_max_limit, void (*fun)(menu_state_t), menu_state_t param){
	if(((touch_data.y_axis > y_min_limit) && (touch_data.y_axis < y_max_limit)) && ((touch_data.x_axis > x_min_limit) && (touch_data.x_axis < x_max_limit))){
		fun(param);
		set_pushed_filter();

	}


}



touch_data_t getTouchData(){
	return touch_data;
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







