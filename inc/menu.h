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



typedef enum {ZEGAR_MENU = 0, BUDZIK_MENU, USTAWIENIA_MENU, INNE_MENU } menu_state_t;

void create_menu();

void show_menu();

void create_firts_submenu(unsigned char *menu_title);


#endif /* MENU_H_ */
