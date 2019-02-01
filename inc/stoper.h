/*
 * stoper.h
 *
 *  Created on: 29.01.2019
 *      Author: gwarchol
 */

#ifndef STOPER_H_
#define STOPER_H_
#include "stdbool.h"



void set_T1_Timer(unsigned char no);
void set_T2_Timer(unsigned char no);

void get_T1_Timer( unsigned char * text);
void get_T2_Timer( unsigned char * text);


void T1_Timer_start(bool x);
void T2_Timer_start(bool x);


bool is_t1_Timer_empty();
bool is_t2_Timer_empty();

void set_T1_Timer_reset();
void set_T2_Timer_reset();


#endif /* STOPER_H_ */
