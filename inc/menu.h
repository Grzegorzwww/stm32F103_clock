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
#include "stdbool.h"
#include "bmp180.h"

#include "string.h"


typedef enum {ZEGAR_MENU = 0, BUDZIK_MENU, USTAWIENIA_MENU, STOPER_MENU, DISPLAY_MESSAGE } menu_state_t;

typedef enum {SET_CLK_HOURS = 0, SET_CLK_MINUTES = 1, SET_CLK_SEK = 2, NO_SET_CLK = 3 } clk_set_state_t;
typedef enum {SET_DATE_DAY = 0, SET_DATE_MONTH = 1, SET_DATE_YEAR = 2, NO_SET_DATE = 3 } date_set_stete_t;


typedef enum {COUNTER_SET = 0, COUNTER_NOT_SET = 1 } countdown_1_stete_t;



void create_menu();

void show_menu();


void display_info_message(unsigned char *msg, unsigned int duration);

unsigned char * set_adjustable_element_on_blinking(int element_no, unsigned char *str, int period, bool year);
void set_record_on_blinking(unsigned char *str, int period);



void increment_set_clk_state();
void increment_set_date_state();
void increment_set_alarm_state();
void control_alarm_off_state();



void T1_Timer_state();
void T2_Timer_state();

void setMenuState(menu_state_t state);

void on_set_up();
void on_set_down();
void on_set_alarm_up();
void on_set_alarm_down();



void on_set_working_day();
void on_set_alarm();

//void on_set_working_day_off();

void on_stoper_menu_presed();
void reset_time_flags();


menu_state_t get_menu_state();
void on_set_saver();

void T1_Timer_reset();
void T2_Timer_reset();


void create_firts_submenu(unsigned char *menu_title);

#endif /* MENU_H_ */
