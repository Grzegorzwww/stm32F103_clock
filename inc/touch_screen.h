/*
 * touch_screen.h
 *
 *  Created on: 26.02.2018
 *      Author: gwarchol
 */

#ifndef TOUCH_SCREEN_H_
#define TOUCH_SCREEN_H_
#include "stm32f10x_spi.h"


#define BufferSize 32


#define SPIz                    SPI2
#define SPIz_CLK                RCC_APB1Periph_SPI2
#define SPIz_GPIO               GPIOB
#define SPIz_GPIO_CLK           RCC_APB2Periph_GPIOB
#define SPIz_PIN_NSS            GPIO_Pin_12
#define SPIz_PIN_SCK            GPIO_Pin_13
#define SPIz_PIN_MISO           GPIO_Pin_14
#define SPIz_PIN_MOSI           GPIO_Pin_15



void init_spi();


#endif /* TOUCH_SCREEN_H_ */
