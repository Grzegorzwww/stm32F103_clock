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
#include "DFPPlayer.h"
#include "device_config.h"
#include "timers.h"
#include "usart_dma.h"
#include "uart_interrupt.h"
#include "scan_timer.h"
#include "touch_screen.h"

#include "rtc.h"
#include "text.h"
#include "itoa.h"



#ifdef DEBUG
extern void initialise_monitor_handles(void);
#endif

unsigned char AddCRC(unsigned char crc,unsigned char b);



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
  	turnOnOffLed(false);
  	init_bmp180();
    LCD_init();
    create_menu();
    init_touch_screen();
	init_timer();

	//timer_2_init();


	rtc_init();

	sleep_mode_init();
	uart_interrup_init();

	//usart_dma_init();

	delay_ms(500);

//	MP3_init();
//	MP3_send_cmd(MP3_VOLUME, 0, 25); // Volume 0-30
//	delay_ms(100);
//	MP3_set_folder(1);

	//MP3_say(MP3_NO_VALUE, 3148, MP3_NO_VALUE);



	save_time(8, 0,0);
	save_date(28,3,2018);
	set_alarm(10, 0, 0);

	int counter  = 0;
//	delay_ms(100);
//	MP3_play_sound(1);



	while (1) {
		__WFI();

//		controlUartTransfer();
//		analizeIncomingDMAData();



		if(getTimerChannelState(TIMER_10ms)){

//			 analizeIncomingData();
			setTimerChannelState(TIMER_10ms, false);
		}

		if (getTimerChannelState(TIMER_100ms)) {


			analize_data_from_touch_screen(true);
			control_touch_buttons();
			show_menu();

			setTimerChannelState(TIMER_100ms, false);
		}

		if (getTimerChannelState(TIMER_1s)) {

			//MP3_queue_processing();
			analize_clock_clendar_state();
			read_environmental_parameters();
			control__goto_sleep_mode();

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




