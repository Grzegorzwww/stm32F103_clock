/*
 * menu.c
 *
 *  Created on: 22.03.2018
 *      Author: gwarchol
 */


#include "menu.h"



void create_menu(){


    LCD_fillScreen(BLACK);
    LCD_setTextBgColor(WHITE );
    LCD_setTextSize(5);




    LCD_fillRect(0, 200, 80, 40, RED);
    LCD_fillRect(80, 200, 80, 40, GREEN);
    LCD_fillRect(160, 200, 80, 40, YELLOW);
    LCD_fillRect(240, 200, 80, 40, BLUE);

    LCD_setTextSize(3);
    LCD_setTextBgColor(RED);
    LCD_setTextColor(WHITE);
    LCD_setCursor(10, 210);

    LCD_writeString("CLOCK");





}

void create_firts_submenu(unsigned char *menu_title){






}
