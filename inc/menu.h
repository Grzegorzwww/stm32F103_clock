/*
 * menu.h
 *
 *  Created on: 22.03.2018
 *      Author: gwarchol
 */

#ifndef MENU_H_
#define MENU_H_


#include "text.h"
#include "itoa.h"
#include "bmp180.h"


typedef enum {ZEGAR_MENU = 0, BUDZIK_MENU, USTAWIENIA_MENU, INNE_MENU } menu_state_t;


typedef enum {SET_CLK_HOURS = 0, SET_CLK_MINUTES = 1, SET_CLK_SEK = 2, NO_SET_CLK = 3 } clk_set_state_t;
typedef enum {SET_DATE_DAY = 0, SET_DATE_MONTH = 1, SET_DATE_YEAR = 2, NO_SET_DATE = 3 } date_set_stete_t;



void create_menu();

void show_menu();


void increment_set_clk_state();
void increment_set_date_state();

void on_set_up();
void on_set_down();


menu_state_t get_menu_state();


void create_firts_submenu(unsigned char *menu_title);

#endif /* MENU_H_ */
