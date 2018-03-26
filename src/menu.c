/*
 * menu.c
 *
 *  Created on: 22.03.2018
 *      Author: gwarchol
 */


#include "menu.h"


static menu_state_t menu_state;



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
		LCD_setTextSize(5);

	    LCD_setTextColor(WHITE);
	    LCD_setTextBgColor(BLACK);
	    LCD_setCursor(20, 0);
		LCD_writeString("USTAWIENIA");

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
