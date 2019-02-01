/*
 * menu.c
 *
 *  Created on: 22.03.2018
 *      Author: gwarchol
 */


#include "menu.h"
#include "rtc.h"
#include "touch_screen.h"
#include "stoper.h"



static menu_state_t menu_state;
static menu_state_t prv_menu_state;

static bool is_button_pushed_up = false;
static bool is_button_pushed_down = false;
static bool is_switch_alarm = false;
static bool is_switch_days = false;



static clk_set_state_t clk_set_state = NO_SET_CLK;
static clk_set_state_t alarm_set_state = NO_SET_CLK;

static date_set_stete_t date_set_stete = NO_SET_DATE;
static countdown_1_stete_t countdown_1_stete = COUNTER_NOT_SET;
static countdown_1_stete_t countdown_2_stete = COUNTER_NOT_SET;

static bool working_days_alarm_flag = false;
static bool on_off_alarm_flag = false;

static unsigned char *msg_ptr;
static unsigned int msg_duration;


bool working_day_checked(){return working_days_alarm_flag;}
bool alarm_on_checked(){return on_off_alarm_flag;}


unsigned char *days[7] = {"Poniedzialek", "Wtorek      ", "Sroda      ", "Czwartek    ", "Piątek     ", "Sobota     ", "Niedziela" };



menu_state_t get_menu_state() {return menu_state;}

void on_button_released(){
	is_button_pushed_up = false;
	is_button_pushed_down = false;
	is_switch_alarm = false;
	is_switch_days = false;
}


void increment_set_clk_state() {
	if(date_set_stete ==  NO_SET_DATE){
		if(clk_set_state >= NO_SET_CLK){
			clk_set_state = SET_CLK_HOURS;
		}else{
			clk_set_state++;
		}
	}
}

void increment_set_date_state() {
	if(clk_set_state ==  NO_SET_CLK){
		if(date_set_stete == SET_DATE_DAY){
			date_set_stete = NO_SET_DATE;
		}else{
			date_set_stete--;
		}
	}
}

void control_alarm_off_state() {
	if(get_alarm_is_working()){
		alarm_stop();
	}
}


void increment_set_alarm_state() {
	if(alarm_set_state >= 3){
		alarm_set_state = 0;
	}else{
		alarm_set_state++;
	}
}

void T1_Timer_state(){

	if(countdown_1_stete == COUNTER_NOT_SET){
		countdown_1_stete = COUNTER_SET;
	}else{
		countdown_1_stete = COUNTER_NOT_SET;
		//if(is_t1_Timer_empty()){
			T1_Timer_start(true);
		//}
	}
}


void T2_Timer_state(){

	if(countdown_2_stete == COUNTER_NOT_SET){
		countdown_2_stete = COUNTER_SET;
	}else{
		countdown_2_stete = COUNTER_NOT_SET;
		//if(is_t2_Timer_empty()){
			T2_Timer_start(true);
		//}
	}
}

void T1_Timer_reset(){
	set_T1_Timer_reset();
}

void T2_Timer_reset(){
	set_T2_Timer_reset();
}




void create_menu(){

    LCD_fillScreen(BLACK);
    LCD_setTextBgColor(WHITE );

    LCD_setTextSize(2);

    LCD_fillRect(0, 200, 80, 40, RED);
    LCD_fillRect(80, 200, 80, 40, GREEN);
    LCD_fillRect(160, 200, 80, 40, YELLOW);
    LCD_fillRect(240, 200, 80, 40, BLUE);

    LCD_setTextBgColor(RED);
    LCD_setTextColor(BLACK);
    LCD_setCursor(10, 210);
    LCD_writeString("ZEGAR");

    LCD_setTextBgColor(GREEN);
    LCD_setTextColor(BLACK);
    LCD_setCursor(86, 210);
    LCD_writeString("BUDZIK");

    LCD_setTextBgColor(YELLOW);
    LCD_setTextColor(BLACK);
    LCD_setCursor(170, 210);
    LCD_writeString("USTAW");

    LCD_setTextBgColor(BLUE);
    LCD_setTextColor(BLACK);
    LCD_setCursor(245, 210);
    LCD_writeString("STOPER");
    menu_state  = ZEGAR_MENU;

}

void setMenuState(menu_state_t state) {
	LCD_fillRect(0, 0, 320, 200, BLACK);
	//LCD_fillScreen(BLACK);
	menu_state = state;
	if(menu_state == STOPER_MENU){
		setAlarmActive(false);
	}
	else {
		setAlarmActive(true);
	}
}

unsigned char temperature_str[10];
unsigned char preassure_str[15];


void read_environmental_parameters(){

	read_temperature_str(temperature_str);
	read_pressure_str(preassure_str);
}

void on_set_working_day(){
	is_switch_days = true;
	if(!working_days_alarm_flag)
		working_days_alarm_flag = true;
	else{
		working_days_alarm_flag = false;
	}
}


void on_set_alarm(){
	is_switch_alarm = true;
	if(!on_off_alarm_flag)
		on_off_alarm_flag = true;
	else{
		on_off_alarm_flag = false;
	}
}



void on_set_alarm_up(){

	is_button_pushed_up = true;
	if(alarm_set_state == SET_CLK_HOURS){
		addAlarmHour();
	}else  if (alarm_set_state == SET_CLK_MINUTES){
		addAlarmMinute();
	}
	else  if (alarm_set_state == SET_CLK_SEK){
		addAlarmSec();
	}
}

void on_set_alarm_down(){
	is_button_pushed_down = true;
	if(alarm_set_state == SET_CLK_HOURS){
		removeAlarmHour();
	}else  if (alarm_set_state == SET_CLK_MINUTES){
		removeAlarmMinute();
	}
	else  if (alarm_set_state == SET_CLK_SEK){
		removeAlarmSec();
	}
}





void on_set_up(){
	 is_button_pushed_up = true;
	if(clk_set_state == SET_CLK_HOURS){
		addHour();
	}else  if (clk_set_state == SET_CLK_MINUTES){
		addMin();
	}
	else  if (clk_set_state == SET_CLK_SEK){
		addSec();
	}

	else if(date_set_stete == SET_DATE_DAY){
		//printf("day up\n");
		addDay();
	}
	else if(date_set_stete == SET_DATE_MONTH){
		//printf("month up\n");
		addMonth();
	}
	else if(date_set_stete == SET_DATE_YEAR){
		//printf("year up\n");
		addYear();
	}
}

static bool saver = false;

void on_set_saver(){

	if(saver){
		saver = false;
	}else{
		saver = true;
	}

}



void on_set_down()
{
	is_button_pushed_down = true;
	if(clk_set_state == SET_CLK_HOURS){
		removeHour();
	}else  if (clk_set_state == SET_CLK_MINUTES){
		removeMin();
	}
	else  if (clk_set_state == SET_CLK_SEK){
		removeSec();
	}

	if(date_set_stete == SET_DATE_DAY){
		removeDay();
	}
	if(date_set_stete == SET_DATE_MONTH){
		removeMonth();
	}
	if(date_set_stete == SET_DATE_YEAR){
		 removeYear();
	}

}



unsigned char * set_adjustable_element_on_blinking(int element_no, unsigned char *str, int period, bool year)
{

	static int prtiod_counter = 0;
	if(prtiod_counter >= period){
			prtiod_counter = 0;
		}
	prtiod_counter++;
	if(prtiod_counter < period / 2  ){
		return str;

	}else if(prtiod_counter > period / 2  ){
		switch(element_no){
		case 0:
			str[0] = ' ';
			str[1] = ' ';
			break;
		case 1:
			str[3] = ' ';
			str[4] = ' ';
			break;
		case 2:
			str[6] = ' ';
			str[7] = ' ';
			if(year){
				str[8] = ' ';
				str[9] = ' ';
			}
			break;
		}
		return str;
	}
}

void set_record_on_blinking(unsigned char *str, int period)
{

	static int prtiod_counter = 0;
	if(prtiod_counter >= period){
			prtiod_counter = 0;
		}
	prtiod_counter++;
	if(prtiod_counter < period / 2  ){
		;

	}else if(prtiod_counter > period / 2  ){

			str[4] = ' ';
			str[5] = ' ';
			str[6] = ' ';
			str[7] = ' ';
			str[8] = ' ';
			str[9] = ' ';
			str[10] = ' ';
			str[11] = ' ';

	}
}

void show_menu(){

	unsigned char timer_str[20];
	unsigned char date_str[20];
	unsigned char alarm_str[20];
	unsigned char counter1_str[20];
	unsigned char counter2_str[20];

	touch_data_t *temp_data_touch = getTouchData();
	static int msg_durration_counter;

	static bool blinking_state = false;


//	getTouchData()

	switch(menu_state)
	{
	case ZEGAR_MENU:


		read_time(timer_str);
		read_date(date_str);




		LCD_setTextSize(6);
	    LCD_setTextColor(WHITE);
	    LCD_setTextBgColor(BLACK);


		LCD_setCursor(20, 0);
		LCD_writeString(timer_str);

		LCD_setTextSize(4);
		LCD_setCursor(50, 75);
		LCD_writeString(date_str);

		LCD_setTextSize(2);
		LCD_setCursor(125, 120);

		LCD_writeString(days[day_of_week()]);

		LCD_setTextSize(2);
		LCD_setCursor(55, 165);
		LCD_writeString(temperature_str);


		LCD_setCursor(170, 165);
		LCD_writeString(preassure_str);



		break;

	case BUDZIK_MENU:
		LCD_setTextSize(3);

		read_alarm(alarm_str);

	    LCD_setTextBgColor(BLACK);
		LCD_setCursor(10, 5);
		LCD_writeString("Alarm: ");


		if(alarm_set_state == SET_CLK_HOURS){ LCD_setTextColor(RED); }
		else if(alarm_set_state== SET_CLK_MINUTES) { LCD_setTextColor(GREEN); }
		else if(alarm_set_state == SET_CLK_SEK) { LCD_setTextColor(YELLOW); }
		else{ LCD_setTextColor(WHITE);}

		LCD_setCursor(140, 5);

		if(alarm_set_state == SET_CLK_HOURS){
			set_adjustable_element_on_blinking(SET_CLK_HOURS, alarm_str, 10, false);
			LCD_writeString(alarm_str);
		}
		else if(alarm_set_state == SET_CLK_MINUTES){
			set_adjustable_element_on_blinking(SET_CLK_MINUTES, alarm_str, 10, false);
			LCD_writeString(alarm_str);
		}
		else if(alarm_set_state == SET_CLK_SEK){
			set_adjustable_element_on_blinking(SET_CLK_SEK, alarm_str, 10, false);
			LCD_writeString(alarm_str);
		}else{
			LCD_writeString(alarm_str);
		}



		LCD_setTextColor(WHITE);
		LCD_setCursor(10, 46);
		LCD_writeString("Alarm:");
		if( is_switch_alarm){LCD_drawRect(230, 42, 80, 31, BLUE);}
		else {LCD_drawRect(230, 42, 80, 31, WHITE);}

		LCD_setTextColor(WHITE);
		LCD_setCursor(233,48);
		if(on_off_alarm_flag){
			LCD_setTextColor(GREEN);
			LCD_writeString("ON  ");

		}else{
			LCD_setTextColor(RED);
			LCD_writeString(" OFF");
		}


		LCD_setTextColor(WHITE);
		LCD_setCursor(10, 87);
		LCD_writeString("Dni robocze:");
		if(is_switch_days){LCD_drawRect(230, 85, 80, 31, BLUE);}
		else{LCD_drawRect(230, 85, 80, 31, WHITE);}

		LCD_setTextColor(WHITE);
		LCD_setCursor(233, 91);
		if(working_days_alarm_flag){
			LCD_setTextColor(GREEN);
			LCD_writeString("ON  ");

		}else{
			LCD_setTextColor(RED);
			LCD_writeString(" OFF");
		}

		LCD_setTextSize(3);
		if(is_button_pushed_up){LCD_drawRect(30, 130, 110, 42, BLUE);}
			else {LCD_drawRect(30, 130, 110, 42, WHITE);}
		LCD_setTextColor(WHITE);
		LCD_setCursor(31, 141);
		LCD_writeString("  UP  ");

		LCD_setTextSize(3);
		if(is_button_pushed_down){LCD_drawRect(170, 130, 110, 42, BLUE);}
			else {LCD_drawRect(170, 130, 110, 42, WHITE);}
		LCD_setTextColor(WHITE);
		LCD_setCursor(171, 141);
		LCD_writeString(" DOWN ");



		break;

	case USTAWIENIA_MENU:


		read_time(timer_str);
		read_date(date_str);

		LCD_setTextSize(3);

		if(clk_set_state == SET_CLK_HOURS){ LCD_setTextColor(RED); }
		else if(clk_set_state == SET_CLK_MINUTES) { LCD_setTextColor(GREEN); }
		else if(clk_set_state == SET_CLK_SEK) { LCD_setTextColor(YELLOW); }
		else{ LCD_setTextColor(WHITE);}

	    LCD_setTextBgColor(BLACK);
		LCD_setCursor(10, 5);
		LCD_writeString("Godz: ");

		LCD_setCursor(120, 5);
//		LCD_writeString(timer_str);

		if(clk_set_state == SET_CLK_HOURS){
				set_adjustable_element_on_blinking(0, timer_str, 10, false);
				LCD_writeString(timer_str);
			}
			else if(clk_set_state == SET_CLK_MINUTES){
				set_adjustable_element_on_blinking(1, timer_str, 10, false);
				LCD_writeString(timer_str);
			}
			else if(clk_set_state == SET_CLK_SEK){
				set_adjustable_element_on_blinking(2, timer_str, 10, false);
				LCD_writeString(timer_str);
			}else{
				LCD_writeString(timer_str);
			}



		if(date_set_stete == SET_DATE_DAY ){ LCD_setTextColor(RED); }
		else if(date_set_stete == SET_DATE_MONTH) { LCD_setTextColor(GREEN); }
		else if(date_set_stete ==  SET_DATE_YEAR) { LCD_setTextColor(YELLOW); }
		else{ LCD_setTextColor(WHITE);}

		LCD_setCursor(10, 70);




		LCD_writeString("Data: ");
		LCD_setCursor(120, 70);
		if(date_set_stete == SET_DATE_DAY){
			set_adjustable_element_on_blinking(SET_DATE_DAY, date_str, 10, false);
			LCD_writeString(date_str);
		}
		else if(date_set_stete == SET_DATE_MONTH){
			set_adjustable_element_on_blinking(SET_DATE_MONTH, date_str, 10, false);
			LCD_writeString(date_str);
		}
		else if(date_set_stete == SET_DATE_YEAR){
			set_adjustable_element_on_blinking(SET_DATE_YEAR, date_str, 10, true);
			LCD_writeString(date_str);
		}else{
			LCD_writeString(date_str);
		}




		LCD_setTextSize(3);
		if(is_button_pushed_up){LCD_drawRect(30, 130, 110, 42, BLUE);}
		else {LCD_drawRect(30, 130, 110, 42, WHITE);}
	    LCD_setTextColor(WHITE);
	    LCD_setCursor(31, 141);
	    LCD_writeString("  UP  ");


	    LCD_setTextSize(3);
		if(is_button_pushed_down){LCD_drawRect(170, 130, 110, 42, BLUE);}
			else {LCD_drawRect(170, 130, 110, 42, WHITE);}
	    LCD_setTextColor(WHITE);
	    LCD_setCursor(171, 141);
	    LCD_writeString(" DOWN ");


		break;
	case STOPER_MENU:


			read_time(timer_str);


			LCD_setTextSize(6);
		    LCD_setTextColor(WHITE);
		    LCD_setTextBgColor(BLACK);

			LCD_setCursor(20, 0);
			LCD_writeString(timer_str);


		    LCD_setTextBgColor(BLACK);
		    LCD_setTextSize(3);
			LCD_setCursor(10, 5);


			LCD_setCursor(65, 80);
			get_T1_Timer(counter1_str);
			if(countdown_1_stete == COUNTER_SET)
				set_record_on_blinking(counter1_str, 10);
			LCD_writeString(counter1_str);

			LCD_setCursor(10, 80);
			LCD_writeString("R");


			LCD_setCursor(65, 145);
			get_T2_Timer(counter2_str);
			if(countdown_2_stete == COUNTER_SET)
				set_record_on_blinking(counter2_str, 10);
			LCD_writeString(counter2_str);

			LCD_setCursor(10, 145);
			LCD_writeString("R");
//			LCD_writeString("T2: 00:00:00");







//		LCD_setTextSize(5);
//
//	    LCD_setTextColor(WHITE);
//	    LCD_setTextBgColor(BLACK);
////	    LCD_setCursor(20, 0);
//	    if(temp_data_touch->y_axis < 105){
//	    	LCD_drawCircle((int)((float)(310 -  temp_data_touch->x_axis ) * 2.8) , (temp_data_touch->y_axis * 1.90), 3 , WHITE);
//	    }
	    //LCD_putPixel((int)((float)(310 -  temp_data_touch->x_axis ) * 2.8) , (temp_data_touch->y_axis * 1.90), WHITE);
	   //LCD_putPixel(310, 210,WHITE);

	   // printf("%d, %d\n", temp_data_touch->x_axis, temp_data_touch->y_axis);
//		LCD_writeString("INNE MENU");

		break;
	case DISPLAY_MESSAGE:
		if(msg_durration_counter++ < msg_duration){
			LCD_setTextBgColor(BLACK);
		LCD_setTextSize(6);
		LCD_setTextColor(RED);

		LCD_setTextSize(4);
		LCD_setCursor(50, 75);
		LCD_writeString(msg_ptr);

		}else{
			if(prv_menu_state == DISPLAY_MESSAGE){
				menu_state = ZEGAR_MENU;
			}else{
				menu_state = prv_menu_state;
			}
			setMenuState(menu_state );
			msg_durration_counter = 0;
			msg_duration = 0;
		}

		break;

	}

}



void display_info_message(unsigned char *msg, unsigned int duration){

	prv_menu_state = menu_state;
	setMenuState(DISPLAY_MESSAGE);


	msg_duration = duration;
	msg_ptr = msg;
}

static unsigned int enter_to_countdown_mode = 1;



void on_stoper_menu_presed(){
	if(enter_to_countdown_mode-- == 0){
		if(menu_state != STOPER_MENU){
			setMenuState(STOPER_MENU);
			enter_to_countdown_mode = 1;
		}
	}
	if(countdown_1_stete == COUNTER_SET){
			set_T1_Timer(30);
	}else{

	}

	if(countdown_2_stete == COUNTER_SET){
			set_T2_Timer(30);
	} else {

	}

}
void reset_time_flags(){
	enter_to_countdown_mode = 1;
}


void create_firts_submenu(unsigned char *menu_title){



}
