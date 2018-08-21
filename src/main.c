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
#include "flash/flash.h"


#ifdef DEBUG
extern void initialise_monitor_handles(void);
#endif

volatile bool state = false;

//unsigned char temp_data[6];

int main(void) {


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

	rtc_init();
	sleep_mode_init();

	delay_ms(500);

	save_time(23, 59, 45);
	save_date(17,9,1988);
	set_alarm(0, 0, 30);

	//FLASH_Init();

	delay_ms(100);

	mp3_init();



	while (1) {
		__WFI();


		analizeIncomingMp3Data();


		if(getTimerChannelState(TIMER_10ms)){
			setTimerChannelState(TIMER_10ms, false);
		}


		if (getTimerChannelState(TIMER_100ms)) {
			analize_data_from_touch_screen(true);
			control_touch_buttons();
			show_menu();
			setTimerChannelState(TIMER_100ms, false);
		}


		if (getTimerChannelState(TIMER_1s)) {
			control_sound_play();
			analize_clock_clendar_state();
			read_environmental_parameters();
			control__goto_sleep_mode();
			setTimerChannelState(TIMER_1s, false);
		}
	}
}




