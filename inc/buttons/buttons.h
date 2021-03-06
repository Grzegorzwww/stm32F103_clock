/*
 * buttons.h
 *
 *  Created on: 24.08.2018
 *      Author: gwarchol
 */

#ifndef BUTTONS_BUTTONS_H_
#define BUTTONS_BUTTONS_H_


#include "stdbool.h"

bool alarm_swich_button;


#define TRUE 1
#define FALSE 0

typedef enum{
	ACTION_DOWN,
	ACTION_UP
} BitEvent;



typedef enum{
	PRESSED,
	RELEASED
}ButtonState;






typedef struct{
	volatile	unsigned int previousNotFilteredState;
	volatile	unsigned int count;
	unsigned int  previousFilteredKeyState;
	unsigned int  numberOfCorrectProbes;
	void (*callback)(BitEvent);
}BitFilterInstance;

//BitFilterInstance ALARM_BUTTON_filter;


//typedef enum{
//	ALARM = 0,
//
//
//}Button;







void initBitFilter(BitFilterInstance *bitFilterInstance, unsigned int numberOfCorrectProbes);
void bitFilter_setOnBitChangeListener_signal(BitFilterInstance *bitFilterInstance, void (*nCallback)(BitEvent));
void buttonIsPressedNotFiltered(BitFilterInstance *bitFilterInstance);
void buttonIsReleasedNotFiltered(BitFilterInstance *bitFilterInstance);
void buttonIsPressedFiltered(BitFilterInstance *bitFilterInstance);
void buttonIsReleasedFiltered(BitFilterInstance *bitFilterInstance);

void init_buttons(void);

void scanButtonsPins(void);




void ALARM_BUTTON_callback(BitEvent bitEvent);
void  ALARM_OFF_BUTTON_callback(BitEvent bitEvent);

void STOPER_ON_BUTTON_callback(BitEvent bitEvent);


#endif /* BUTTONS_BUTTONS_H_ */
