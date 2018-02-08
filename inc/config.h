/*
 * config.h
 *
 *  Created on: 05.12.2017
 *      Author: gwarchol
 */

#ifndef CONFIG_H_
#define CONFIG_H_



#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stdbool.h"





void device_init();
void init_nvic();
void turnOnOffLed(bool state);
void RCC_Conf(void);






#endif /* CONFIG_H_ */
