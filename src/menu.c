/*
 * menu.c
 *
 *  Created on: 22.03.2018
 *      Author: gwarchol
 */


#include "menu.h"


static menu_state_t menu_state;

static clk_set_state_t clk_set_state = NO_SET_CLK;
static date_set_stete_t date_set_stete = NO_SET_DATE;


menu_state_t get_menu_state() {return menu_state;}

void increment_set_clk_state() {
	if(clk_set_state >= 3){
		clk_set_state = 0;
	}else{
		clk_set_state++;
	}
}
void increment_set_date_state() {

	if(date_set_stete >= 3){
		date_set_stete = 0;
	}else{
		date_set_stete++;
	}

}



void create_menu(){


    LCD_fillScreen(BLACK);
    LCD_setTextBgColor(WHITE );
//    LCD_setTextSize(5);



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
    LCD_setCursor(250, 210);
    LCD_writeString("INNE");


    menu_state  = ZEGAR_MENU;


}

void setMenuState(menu_state_t state) {
	LCD_fillRect(0, 0, 310, 200, BLACK);
	//LCD_fillScreen(BLACK);
	menu_state = state;


}

unsigned char temp_str[10];
unsigned char preassure_str[15];


void read_environmental_parameters(){


	read_temperature_str(temp_str);

	read_pressure_str(preassure_str);
}

void on_set_up(){

	if(clk_set_state == SET_CLK_HOURS){
		addHour();
	}else  if (clk_set_state == SET_CLK_MINUTES){
		addMin();
	}
	else  if (clk_set_state == SET_CLK_SEK){
		addSec();
	}



	if(date_set_stete == SET_DATE_DAY){
		//printf("day up\n");
		addDay();
	}
	if(date_set_stete == SET_DATE_MONTH){
		//printf("month up\n");
		addMonth();
	}
	if(date_set_stete == SET_DATE_YEAR){
		//printf("year up\n");
		addYear();
	}



}
void on_set_down()
{

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





void show_menu(){

	unsigned char timer_str[20];
	unsigned char date_str[20];




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
		LCD_setCursor(65, 140);
		LCD_writeString(temp_str);


		LCD_setCursor(170, 140);
		LCD_writeString(preassure_str);



		break;

	case BUDZIK_MENU:
		LCD_setTextSize(5);

	    LCD_setTextColor(WHITE);
	    LCD_setTextBgColor(BLACK);
	    LCD_setCursor(20, 0);
		LCD_writeString("BUDZIK");

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
		LCD_setCursor(10, 20);
		LCD_writeString(timer_str);


		if(date_set_stete == SET_DATE_DAY ){ LCD_setTextColor(RED); }
		else if(date_set_stete == SET_DATE_MONTH) { LCD_setTextColor(GREEN); }
		else if(date_set_stete ==  SET_DATE_YEAR) { LCD_setTextColor(YELLOW); }
		else{ LCD_setTextColor(WHITE);}

		LCD_setCursor(10, 70);
		LCD_writeString(date_str);


	   // LCD_fillRect(200, 20, 55, 30, WHITE);

//	    LCD_setTextBgColor(WHITE);
//	    LCD_setTextColor(BLACK);
//	    LCD_setCursor(185, 20);
//	    LCD_writeString("SET");
//
//
//
//	    //LCD_fillRect(200, 70, 55, 30, WHITE);
//
//	    LCD_setTextBgColor(WHITE);
//	    LCD_setTextColor(BLACK);
//	    LCD_setCursor(185, 70);
//	    LCD_writeString("SET");


		LCD_setTextSize(3);
	    LCD_setTextBgColor(RED);
	    LCD_setTextColor(BLACK);
//	    LCD_setCursor(245, 20);
	    LCD_setCursor(200, 20);
	    LCD_writeString("  UP  ");

	    LCD_setTextBgColor(RED);
	    LCD_setTextColor(BLACK);
//	    LCD_setCursor(245, 70);
	    LCD_setCursor(200, 70);
	    LCD_writeString(" DOWN ");




//		LCD_setTextSize(2);
//		LCD_setCursor(65, 140);
//		LCD_writeString(temp_str);
//
//
//		LCD_setCursor(170, 140);
//		LCD_writeString(preassure_str);
//

//		LCD_setTextSize(5);
//
//	    LCD_setTextColor(WHITE);
//	    LCD_setTextBgColor(BLACK);
//	    LCD_setCursor(20, 0);
//		LCD_writeString("USTAWIENIA");

		break;
	case INNE_MENU:
		LCD_setTextSize(5);

	    LCD_setTextColor(WHITE);
	    LCD_setTextBgColor(BLACK);
	    LCD_setCursor(20, 0);
		LCD_writeString("INNE MENU");

		break;


	}



}






void create_firts_submenu(unsigned char *menu_title){






}
