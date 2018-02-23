/*
 * device_config.h
 *
 *  Created on: 23.02.2018
 *      Author: gwarchol
 */

#ifndef DEVICE_CONFIG_H_
#define DEVICE_CONFIG_H_

/*
 * config.h
 *
 *  Created on: 05.12.2017
 *      Author: gwarchol
 **/



#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stdbool.h"





void device_init();
void init_nvic();
void turnOnOffLed(bool state);
void RCC_Conf(void);






#endif /* DEVICE_CONFIG_H_ *

#endif /* DEVICE_CONFIG_H_ */
