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
#include "touch_screen.h"
#include "rtc.h"


#include "text.h"
#include "itoa.h"


extern void initialise_monitor_handles(void);


unsigned char AddCRC(unsigned char crc,unsigned char b);
//
//#ifdef DEBUG

//#endif

volatile bool state = false;

unsigned char temp_data[6];

int main(void) {

	//SystemInit();


//#ifdef DEBUG
	initialise_monitor_handles();
	//printf("start\n");
//#endif

	RCC_Conf();
	device_init();

  	turnOnOffLed(false);

    LCD_init();
    LCD_setOrientation(ORIENTATION_LANDSCAPE);
    LCD_fillScreen(BLACK);
    LCD_setTextBgColor(WHITE );
    LCD_setTextSize(5);

   init_touch_screen();


	init_timer();

	rtc_init();


//	usart_dma_init();
	//rtc_init();




	unsigned char tabilca[20];
	unsigned char tabilca_date[20];
	int index = 0;

	save_time(23, 59, 30);
	save_date(17,9,1988);

	set_alarm(23, 59, 45);


	while (1) {

		//controlUartTransfer();
		//analizeIncomingDMAData();

		if(getTimerChannelState(TIMER_10ms)){




			setTimerChannelState(TIMER_10ms, false);
		}

		if (getTimerChannelState(TIMER_100ms)) {






			//analize_data_from_touch_screen(true, tabilca);


//			LCD_setCursor(0, 0);
//			LCD_fillRect(0, 0, 100, 24, BLACK);
//			LCD_writeString(tabilca);

//			int ticks = getTotalRtcTicks();
//			int n = sprintf (tablica,"timer ticks = %d\n", ticks);
			//read_timer(tablica);
//			uart_dma_send_data(tablica, 9);
			//analizeIncomingData();
//			touch_screen_send_command(0xB3);
//			touch_screen_transmit_dma_data_set();
//			 touch_screen_receive_dma_data_set();
//			touch_screen_transmit_dma_data_set();





//			printf("X = %d\n", Odczyt_X());
//			printf("Y = %d\n", Odczyt_Y());


			setTimerChannelState(TIMER_100ms, false);
		}

		if (getTimerChannelState(TIMER_1s)) {

			read_time(tabilca);
			read_date(tabilca_date);

			analize_clock_clendar_state();


			LCD_setCursor(0, 0);
			LCD_fillRect(0, 0, 100, 24, BLACK);


			LCD_writeString(tabilca);

			LCD_setCursor(0, 50);
			LCD_writeString(tabilca_date);
//			LCD_setCursor(100, 100);
//			LCD_fillRect(0, 50, 100, 24, BLACK);







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




