/*
 * stoper.c
 *
 *  Created on: 29.01.2019
 *      Author: gwarchol
 */


#include "stoper.h"

static unsigned int seconds_T1_Timer = 0;
static unsigned int seconds_T2_Timer = 0;

bool t1_Timer_start = false;
bool t2_Timer_start = false;

bool alarm_found_t1 = false;
bool alarm_found_t2 = false;

bool is_alarm_found_t1() {return alarm_found_t1;}
bool is_alarm_found_t2() {return alarm_found_t2;}


void T1_Timer_start(bool x){
	t1_Timer_start = x;
}
void T2_Timer_start(bool x){
	t2_Timer_start = x;
}

bool is_t1_Timer_empty(){
	return (seconds_T1_Timer == 0);
}

bool is_t2_Timer_empty(){
	return (seconds_T2_Timer == 0);
}


void set_T1_Timer(unsigned char no){
	seconds_T1_Timer += no;
}
void set_T2_Timer(unsigned char no){
	seconds_T2_Timer += no;
}


void set_T1_Timer_reset(){

	seconds_T1_Timer = 0;
}

void set_T2_Timer_reset(){
	seconds_T2_Timer = 0;

}



void decrement_Counters(){
	if(seconds_T1_Timer > 0 && t1_Timer_start ){
		seconds_T1_Timer--;
		if(seconds_T1_Timer == 0){
			t1_Timer_start = false;
			play_alarm(6, 2);

		}
	}
	if(seconds_T2_Timer > 0 && t2_Timer_start ){
			seconds_T2_Timer--;
			if(seconds_T2_Timer == 0){
				t2_Timer_start = false;
				play_alarm(7, 1);
			}
		}

}

void get_T1_Timer( unsigned char * text){

	int min = seconds_T1_Timer / 60;
	int sec = seconds_T1_Timer % 60;
	sprintf(text, "T1: %d%d:%d%d:%d%d",
			0,
			0,
			min / 10,
			min % 10,
			sec / 10,
			sec % 10);


}


void get_T2_Timer(unsigned char * text){

	int min = seconds_T2_Timer / 60;
	int sec = seconds_T2_Timer % 60;
	sprintf(text, "T2: %d%d:%d%d:%d%d",
			0,
			0,
			min / 10,
			min % 10,
			sec / 10,
			sec % 10);

}

