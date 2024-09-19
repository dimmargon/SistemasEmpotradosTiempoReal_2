/*
 * led_driver.c
 *
 *  Created on: Sep 13, 2024
 *      Author: dimasPortatil
 */
#include "stm32l4xx_hal.h"
#include "led_driver.h"


void LED_On(uint8_t nLed){
	switch(nLed){
	case 0:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, SET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, SET);
		break;
	}
}

void LED_Off(uint8_t nLed){
	switch(nLed){
	case 0:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, RESET);
		break;
	}
}

void LED_Toggle(uint8_t nLed){
	switch(nLed){
	case 0:
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		break;
	case 1:
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
		break;
	case 2:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
		break;
	}
}
