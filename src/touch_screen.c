/*
 * touch_screen.h
 *
 *  Created on: 26.02.2018
 *      Author: gwarchol
 */

#include "touch_screen.h"

SPI_InitTypeDef SPI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;






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





	// Konfiguracja SPI2
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



		// Włączenie SPI2
		SPI_Cmd(SPI2, ENABLE);


			SPI_I2S_ITConfig(SPI_2, SPI_I2S_IT_RXNE, ENABLE);

			NVIC_InitTypeDef NVIC_InitStructure;

			/* 1 bit for pre-emption priority, 3 bits for subpriority */
			//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

			/* Configure and enable SPI_MASTER interrupt -------------------------------*/
			NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);



			touch_screen_cs_high();
}




static void Wyslij_Komende (char cmd)
{
	/* Oczekiwanie, aż bufor wyjściowy TX będzie pusty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	/* Wysyłanie komendy po SPI*/
	SPI_I2S_SendData(SPI2,cmd);
	/* Oczekiwanie na odbiór danych z SPI */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	/* Odczyt odebranych danych */
	SPI_I2S_ReceiveData(SPI2);
}

int Odczytaj_Pomiar(void)
{
	unsigned short buf,temp;
	/* Oczekiwanie, aż bufor wyjściowy TX będzie pusty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	/* Wysyłanie komendy wyzwalającej pomiar */
	SPI_I2S_SendData(SPI2,0x0000);
	/* Oczekiwanie na odbiór 8 starszych bitów wykonanego pomiaru dla 12bit konwersji */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	/* Odczyt otrzymanych danych */
	temp=SPI_I2S_ReceiveData(SPI2);
	/* Zapisywanie danych do bufora */
	buf=temp<<8;
	//DelayUS(1);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	/* Wysyłanie komendy wyzwalającej pomiar */
	SPI_I2S_SendData(SPI2,0x0000);
	/* Oczekiwanie na odbiór 4 starszych bitów wykonanego pomiaru dal 12bit konwersji */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	/* Odczyt odebranych danych */
	temp=SPI_I2S_ReceiveData(SPI2);
	/* Łączenie otrzymanych danych w 12bitową liczbę */
	buf |= temp;
	buf>>=3;
	buf&=0xfff;
	return buf;
}

int Odczyt_X(void)
{
	int i;
	// linia CS jest ustawiana w stan niski
	 touch_screen_cs_low();
	// opóźnienie potrzebne do aktywacji kontrolera
	 delay_us(1);
	// Wysłanie komendy ‘10010000’
	Wyslij_Komende (0b10010000);
	// opóźnienie związane z odczytem napiecia
	delay_us(20);
	// odczyt napięcia
	i=Odczytaj_Pomiar();
	// ustawienie linii CS w stan niski
	touch_screen_cs_high();
	// konwersja odczytanego napięcia
	return ( 320 - i/12+18 );
}


/* Odczyt pozycji Y z panelu dotykowego */
int Odczyt_Y(void)
{
	int i;
	// linia CS jest ustawiana w stan niski
	touch_screen_cs_low();
	// opóźnienie potrzebne do aktywacji kontrolera
	delay_us(1);
	// Wysłanie komendy ‘11010000’
	Wyslij_Komende (0b11010000);
	// opóźnienie związane z odczytem napiecia
	delay_us(20);
	// odczyt napięcia
	i=Odczytaj_Pomiar();
	// ustawienie linii CS w stan niski
	touch_screen_cs_high();
	// konwersja odczytanego napięcia
	return i/17;
}


void send_to_touch_screen(unsigned short command){

//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
//	/* Wysyłanie komendy po SPI*/
//	SPI_I2S_SendData(SPI2,command);


	/* Oczekiwanie na odbiór danych z SPI */
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
//	/* Odczyt odebranych danych */
//	SPI_I2S_ReceiveData(SPI2);


}


void init_touch_screen(){







}



void SPI2_IRQHandler(void) {
    if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET) {
        //Send from buffer
    	printf("przerwanie SPI TXE\n");
        SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
    }

    if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET) {
        //Receive
    	printf("przerwanie SPI RXE\n");
    	printf("%d\n", SPI_I2S_ReceiveData(SPI2));
        SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
    }
}

void touch_screen_send_command(unsigned short command){

	touch_screen_cs_low();
	//while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
			/* Wysyłanie komendy po SPI*/
			SPI_I2S_SendData(SPI2,command);

			touch_screen_cs_high();
}



 void touch_screen_cs_low(void){
	GPIO_WriteBit(SPI_2, SPI_2_PIN_NSS,Bit_RESET );
}

 void touch_screen_cs_high(void){
	GPIO_WriteBit(SPI_2, SPI_2_PIN_NSS,Bit_SET );
}







