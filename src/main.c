/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */


 #define HSE_VALUE    ((uint32_t)8000000)


#include "stm32f10x.h"
#include "device_config.h"
#include "timers.h"
#include "usart_dma.h"
#include "uart_interrupt.h"
#include "scan_timer.h"
#include "rtc.h"


#include "text.h"
#include "itoa.h"




unsigned char AddCRC(unsigned char crc,unsigned char b);

#ifdef DEBUG
extern void initialise_monitor_handles(void);
#endif

volatile bool state = false;



unsigned char temp_data[6];

int main(void) {

	//SystemInit();


#ifdef DEBUG
	initialise_monitor_handles();
	printf("start\n");
#endif

	RCC_Conf();
	device_init();


    LCD_init();
    LCD_setOrientation(ORIENTATION_PORTRAIT);
    LCD_fillScreen(BLACK);

    char buf[16];
    LCD_setTextBgColor(DGREEN);
    LCD_setTextSize(3);



	volatile bool x = false;
	volatile bool y = false;

	init_timer();
//	uart_interrup_init();
	usart_dma_init();


	rtc_init();


	unsigned char _crc = 0;


	unsigned char tablica[14];
	int i;
	for(i = 0; i < 14; i++){
		tablica[i] = 0;
	}



	while (1) {

		controlUartTransfer();
		analizeIncomingDMAData();

		if (getTimerChannelState(TIMER_100ms)) {
			if (y == true) {
				GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
				y = false;
			} else if (y == false) {
				GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
				y = true;
			}

//			int ticks = getTotalRtcTicks();
//			int n = sprintf (tablica,"timer ticks = %d\n", ticks);




			//read_timer(tablica);

			_crc = 0;

			tablica[0] = 0x3C;
			tablica[1] = 0xA5;
			tablica[2] = 0x01;
			tablica[3] = 0xAA;
			tablica[4] = 0xDB;
			tablica[5] = 0xDB;
			tablica[6] = 0xDB;
			tablica[7] = 0xDB;



			for(i = 0; i < 8; i++){
				_crc = AddCRC(_crc, tablica[i]);

			}
			tablica[8] = _crc;










			uart_dma_send_data(tablica, 9);




			//analizeIncomingData();
			//uart_send_data("Hello world\n", 12);
			//uart_send_data(temp_tab, 12);

			setTimerChannelState(TIMER_100ms, false);
		}

		if (getTimerChannelState(TIMER_1s)) {



			setTimerChannelState(TIMER_1s, false);
		}
	}
}


unsigned char AddCRC(unsigned char crc,unsigned char b)
{
	unsigned char i;
	unsigned char c;
	i = 0;
	do
	{
		c = crc ^ b;
		crc = crc >> 1;
		if((c & 0x01) != 0 )
			crc = crc ^ 0x8C;
		b = b >> 1;
	}while(++i < 8);
	return crc;
}




