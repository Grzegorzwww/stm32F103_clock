/*
 * touch_screen.h
 *
 *  Created on: 26.02.2018
 *      Author: gwarchol
 */

#ifndef TOUCH_SCREEN_H_
#define TOUCH_SCREEN_H_
#include "stm32f10x_spi.h"
#include "stm32f10x_it.h"


#define BufferSize 32


#define SPI_2                    SPI2
#define SPI_2_CLK                RCC_APB1Periph_SPI2
#define SPI_2_GPIO               GPIOB
#define SPI_2_GPIO_CLK           RCC_APB2Periph_GPIOB
#define SPI_2_PIN_NSS            GPIO_Pin_12
#define SPI_2_PIN_SCK            GPIO_Pin_13
#define SPI_2_PIN_MISO           GPIO_Pin_14
#define SPI_2_PIN_MOSI           GPIO_Pin_15



void init_spi();


#endif /* TOUCH_SCREEN_H_ */
