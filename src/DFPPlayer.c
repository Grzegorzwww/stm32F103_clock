/*
 * DFPPlayer.c
 *
 *  Created on: 05.04.2018
 *      Author: gwarchol
 */


#include "DFPPlayer.h"
#include "uart_interrupt.h"

/* MP3 LIB
 * V 0.1, Andrey Koryagin, Kremenchuk, Ukraine, 2016
 * Simple library to use the DFPlayer.
 * It's can say a numbers from -9999 to 9999.
 * Before using need to copy specially prepared mp3-files to SD-card.
 * You can find a sample of this files in folder "SD_CARD_FILES"
 * You can make same folders for different languages and choose language for playing.
 * To choose a language (folder), please use MP3_set_folder function.
 * You can free use and modify this lib as you like. Good luck.
 *
 *
 */



volatile uint8_t mp3_RXi;
volatile char mp3_RXc;
volatile char mp3_RX_Buf[mp3_RX_Buf_SIZE] = {'\0'};
volatile uint8_t mp3_folder = 1;
volatile uint8_t mp3_cmd_buf[10] = {0x7E, 0xFF, 0x06, 0x00, 0x01, 0x0, 0x0, 0x00, 0x00, 0xEF};
volatile uint8_t mp3_queue[MP3_QUEUE_LEN] = {MP3_NO_VALUE, MP3_NO_VALUE, MP3_NO_VALUE, MP3_NO_VALUE, MP3_NO_VALUE, MP3_NO_VALUE, MP3_NO_VALUE, MP3_NO_VALUE, MP3_NO_VALUE, MP3_NO_VALUE};
volatile int8_t mp3_queue_id = 0;
volatile uint8_t mp3_track_playing_finished = 0;


volatile int play_alarm_couner = 0;
volatile int play_alarm_id = 1;
volatile bool alarm_is_working = false;
volatile bool alarm_snooze_is_working = false;
unsigned long actual_set_alarm;


bool get_alarm_is_working() {return alarm_is_working;}

/* UART3 Initialization for DFPlayer using
 * Communication Standard: 9600 bps
 */
void mp3_init(void)
{

	uart_interrup_init();
	//uart_send_data(" ", 1);

	mp3_send_cmd(MP3_VOLUME, 0, 30); // Volume 0-30
	delay_ms(10);


	MP3_clear_RXBuffer();

	actual_set_alarm = RTC_GetAlarm();


	//MP3_set_folder(5);
	//delay_ms(10);

}


void MP3_clear_RXBuffer(void) {
	for (mp3_RXi=0; mp3_RXi<mp3_RX_Buf_SIZE; mp3_RXi++)
		mp3_RX_Buf[mp3_RXi] = '\0';
	mp3_RXi = 0;
}

/* Processing data received from a DFPlayer
 * We need to know the moment of the end of track playing.
 * Sets a flag when message received and the end of track playing is detected.
 * Other messages are ignored.
 */


void analizeDataFromMP3(unsigned char data ){

	mp3_RXc = data;
	//	printf("%x\n", mp3_RXc);
	mp3_RX_Buf[mp3_RXi] = mp3_RXc;
	mp3_RXi++;

	if (mp3_RXc != 0xEF) { // End of DFPlayer message
		if (mp3_RXi > mp3_RX_Buf_SIZE-1) {
			MP3_clear_RXBuffer();
		}
	}
	else {
		//printf("%x\n", mp3_RX_Buf[3]);
		switch(mp3_RX_Buf[3]){

		case MP3_Q_STAY2:
			mp3_track_playing_finished = 1;
			break;

		case MP3_Q_ERROR:
			//ERROR
			break;
		}
		MP3_clear_RXBuffer();
	}
}


uint16_t MP3_checksum (void) {
	uint16_t sum = 0;
	uint8_t i;
	for (i=1; i<7; i++) {
		sum += mp3_cmd_buf[i];
	}
	return -sum;
}

/* Send command to DFPlayer
 */
void mp3_send_cmd (uint8_t cmd, uint16_t high_arg, uint16_t low_arg) {
	uint8_t i;
	uint16_t checksum;

	mp3_cmd_buf[3] = cmd;

	mp3_cmd_buf[5] = high_arg;
	mp3_cmd_buf[6] = low_arg;

	checksum = MP3_checksum();
	mp3_cmd_buf[7] = (uint8_t) ((checksum >> 8) & 0x00FF);
	mp3_cmd_buf[8] = (uint8_t) (checksum & 0x00FF);

	// Send command to UART3
	uart_send_data(mp3_cmd_buf, 10);


}

/* This function make a queue from simple mp3-files to say a long number.
 * Range -9999...9999
 * You can use prefix and suffix. It's a mp3-files which will play before and after number.
 */
void MP3_say(uint8_t prefix, int value, uint8_t suffix) {
	int i;
	int num;

	const int POW[4] = {1, 10, 100, 1000}; // Range -9999...9999

	// Erase MP3 Queue
	for (i=0; i<MP3_QUEUE_LEN; i++) {
		mp3_queue[i] = MP3_NO_VALUE;
	}

	// Fill MP3 Queue
	mp3_queue_id = -1;

	// Prefix
	if (prefix != MP3_NO_VALUE) {
		mp3_queue_id++;
		mp3_queue[mp3_queue_id] = prefix;
	}

	if (value < 0) {
		value = abs(value);
		mp3_queue_id++;
		mp3_queue[mp3_queue_id] = 250; // Minus
	}
	else {
		if (value == 0) {
			mp3_queue_id++;
			mp3_queue[mp3_queue_id] = 200;
		}
	}

	for (i = 3; i >= 0; i--) { // Range -9999...9999
		if (value > 19 ) {
			num = value / POW[i];
			value = value - num * POW[i];

			if (num > 0) {
				mp3_queue_id++;
				mp3_queue[mp3_queue_id] = (i+1)*10 + num;
				//mp3_queue_id++;
			}
		}
		else {
			if (value > 0) {
				mp3_queue_id++;
				mp3_queue[mp3_queue_id] = value;
				value = 0;
			}
		}
	}

	// Suffix
	if (suffix != MP3_NO_VALUE) {
		mp3_queue_id++;
		mp3_queue[mp3_queue_id] = suffix;
	}

	mp3_queue_id = 0;
	mp3_track_playing_finished = 1; // Ready to play
}



void MP3_play_sound(unsigned char no)
{
	if (mp3_track_playing_finished == 1) {
		mp3_send_cmd(MP3_PLAY_FOLDER_FILE, mp3_folder, mp3_queue[no]);
		MP3_clear_RXBuffer();
		mp3_track_playing_finished = 0;
	}
}




void MP3_set_folder (uint8_t folder) {
	mp3_folder = folder;
}


void play_alarm(int alarm_no, int num_of_repet ){

	mp3_track_playing_finished = 1;
	play_alarm_couner = num_of_repet;
	play_alarm_id = alarm_no;
	alarm_is_working = true;

	control_sound_play();
}

static bool alarm_is_acive = true;

void setAlarmActive(bool x){
	alarm_is_acive = x;
}

bool isAlarmActive(){
	return alarm_is_acive;
}

void alarm_stop(){
	if(alarm_is_acive){
		if(alarm_snooze_is_working && alarm_is_working ){
			RTC_SetAlarm(actual_set_alarm);
			alarm_snooze_is_working = false;
			display_info_message("ALARM WYL", 15);
			mp3_send_cmd(MP3_RESET, 0, 0);
		}else if(alarm_snooze_is_working && !alarm_is_working){
			alarm_snooze_is_working = false;
			display_info_message("ALARM WYL", 15);
			mp3_send_cmd(MP3_RESET, 0, 0);
			RTC_SetAlarm(actual_set_alarm);
		}
		mp3_track_playing_finished = 0;
		play_alarm_couner = 0;
		alarm_is_working = false;

	}
}


void alarm_snooze(){
	if(alarm_is_acive){
		if(alarm_is_working ){
			display_info_message(" DRZEMKA ", 10);
			mp3_send_cmd(MP3_RESET, 0, 0);
			addAlarmSnooze();
		}
		mp3_track_playing_finished = 0;
		play_alarm_couner = 0;
		alarm_is_working = false;
		alarm_snooze_is_working = true;
	}

}

void control_sound_play(){
	if(mp3_track_playing_finished == 1){
		if(play_alarm_couner > 0){
			mp3_send_cmd(MP3_PLAY_FOLDER_FILE, 5, play_alarm_id);
			play_alarm_couner--;
		}else{
			alarm_is_working = false;
		}
		mp3_track_playing_finished = 0;
	}
}



