/*
 * joy_driver.c
 *
 *  Created on: Sep 19, 2024
 *      Author: dimasPortatil
 */
#include "stm32l4xx_hal.h"

uint8_t ReadJoy(void) {
	static uint8_t button = 0;
	static uint8_t prevButtonState = GPIO_PIN_SET;

	uint8_t currentButtonState = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	if (currentButtonState == GPIO_PIN_RESET
			&& prevButtonState == GPIO_PIN_SET) {
		button++;
		prevButtonState = GPIO_PIN_RESET;
	} else if (currentButtonState == GPIO_PIN_SET
			&& prevButtonState == GPIO_PIN_RESET) {
		prevButtonState = GPIO_PIN_SET;
	}

	if (button > 4) {
		button = 0;
	}

	return button;
}
