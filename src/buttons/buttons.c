/*
 * buttons.c
 *
 *  Created on: 24.08.2018
 *      Author: gwarchol
 */

#include "buttons/buttons.h"

#include "stm32f10x.h"


/*
* buttons_filter.c
*
*  Created on: 23 mar 2016
*      Author: warchol
*/




BitFilterInstance ALARM_BUTTON_filter;
BitFilterInstance ALARM_OFF_BUTTON_filter;






void init_buttons(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	initBitFilter(&ALARM_BUTTON_filter, 5);
	initBitFilter(&ALARM_OFF_BUTTON_filter, 255);


	bitFilter_setOnBitChangeListener_signal(&ALARM_BUTTON_filter,ALARM_BUTTON_callback);
	bitFilter_setOnBitChangeListener_signal(&ALARM_OFF_BUTTON_filter,ALARM_OFF_BUTTON_callback);

}

void scanButtonsPins(void) {

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)== Bit_SET){
		buttonIsPressedNotFiltered(&ALARM_BUTTON_filter);
		buttonIsPressedNotFiltered(&ALARM_OFF_BUTTON_filter);
	}
	else {
		buttonIsReleasedNotFiltered(&ALARM_BUTTON_filter);
		buttonIsReleasedNotFiltered(&ALARM_OFF_BUTTON_filter);
	}
}


void ALARM_BUTTON_callback(BitEvent bitEvent){
	switch(bitEvent){
		case ACTION_UP:
			//printf("Alarm button up\n");

		break;
		case ACTION_DOWN:
			printf("Alarm snooze\n");
			alarm_snooze();

		break;
	}
}
void ALARM_OFF_BUTTON_callback(BitEvent bitEvent){
	switch(bitEvent){
		case ACTION_UP:


		break;
		case ACTION_DOWN:
			printf("Alarm off \n");
			alarm_stop();

		break;
	}
}





static void nullCallback(BitEvent bitEvent){}


void initBitFilter(BitFilterInstance *bitFilterInstance, unsigned char  numberOfCorrectProbes){
	bitFilterInstance->callback = nullCallback;
	bitFilterInstance->previousFilteredKeyState = 2; // 2 - not defined, used for first scan
	bitFilterInstance->previousNotFilteredState = 0;
	bitFilterInstance->count = 0;
	bitFilterInstance->numberOfCorrectProbes = numberOfCorrectProbes;
}

void bitFilter_setOnBitChangeListener_signal(BitFilterInstance *bitFilterInstance, void (*nCallback)(BitEvent)){
	bitFilterInstance->callback = nCallback;
}


void callbackEventPressed(BitFilterInstance *bitFilterInstance){
	bitFilterInstance->callback(ACTION_DOWN);
}

void callbackEventReleased(BitFilterInstance *bitFilterInstance){
	bitFilterInstance->callback(ACTION_UP);
}

void buttonIsPressedFiltered(BitFilterInstance *bitFilterInstance){
	if(bitFilterInstance->previousFilteredKeyState != 1){
		bitFilterInstance->previousFilteredKeyState = 1;
		callbackEventPressed(bitFilterInstance);
	}
}

void buttonIsReleasedFiltered(BitFilterInstance *bitFilterInstance){
	if(bitFilterInstance->previousFilteredKeyState != 0){
		bitFilterInstance->previousFilteredKeyState = 0;
		callbackEventReleased(bitFilterInstance);
	}
}


void incrementPressedProbe(BitFilterInstance *bitFilterInstance){

	if(bitFilterInstance->previousNotFilteredState == 1){
		bitFilterInstance->count++;
		if(bitFilterInstance->count == bitFilterInstance->numberOfCorrectProbes){
			buttonIsPressedFiltered(bitFilterInstance);
			bitFilterInstance->count = 1;
		}
		}else{
		bitFilterInstance->previousNotFilteredState = 1;
		bitFilterInstance->count = 1;
	}
}

void incrementReleasedProbe(BitFilterInstance *bitFilterInstance){
	if(bitFilterInstance->previousNotFilteredState == 0){
		bitFilterInstance->count++;
		if(bitFilterInstance->count == bitFilterInstance->numberOfCorrectProbes){
			buttonIsReleasedFiltered(bitFilterInstance);
			bitFilterInstance->count = 1;
		}
		}else{
		bitFilterInstance->previousNotFilteredState = 0;
		bitFilterInstance->count = 1;
	}
}

void buttonIsPressedNotFiltered(BitFilterInstance *bitFilterInstance){
	incrementPressedProbe(bitFilterInstance);
}

void buttonIsReleasedNotFiltered(BitFilterInstance *bitFilterInstance){
	incrementReleasedProbe(bitFilterInstance);
}

