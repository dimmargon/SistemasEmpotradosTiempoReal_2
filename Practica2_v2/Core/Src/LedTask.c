/*
 * LedTask.c
 *
 *  Created on: Sep 20, 2024
 *      Author: dimasPortatil
 */
#include "stm32l4xx_hal.h"
#include "led_driver.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "joy_driver.h"
#include "LedTask.h"

/*
 * EJERCICIO 1 y 2
 */
//led0 = 0 #todos los leds a la vez
int led1 = 1;
int led2 = 2;
int led3 = 3;

int nButton = 0;

/*
 * EJERCICIO 3
 */
int led = 1;

/*
 * EJERCICIO 6
 */
TaskHandle_t taskHandler1;
TaskHandle_t taskHandler2;
TaskHandle_t taskHandler3;

extern uint8_t numPulsaciones;


void CreateLedTask(){
	/*
	 * EJERCICIOS 1 y 2
	 */
//	xTaskCreate(LedToggleTask1, "LedToogleTask1", 128, &led1, 1, NULL);
//	xTaskCreate(LedToggleTask1, "LedToogleTask2", 128, &led2, 1, NULL);
//	xTaskCreate(LedToggleTask1, "LedToogleTask3", 128, &led3, 1, NULL);
//	xTaskCreate(LedToggleTask2, "LedToogleTask1", 128, &led1, 1, NULL);
//	xTaskCreate(LedToggleTask2, "LedToogleTask2", 128, &led2, 1, NULL);
//	xTaskCreate(LedToggleTask2, "LedToogleTask3", 128, &led3, 1, NULL);
	/*
	 * EJERCICIO 3
	 */
//	xTaskCreate(LedToggleTask3, "LedToogleTask1", 128, &led, 1, NULL);
	/*
	 * EJERCICIO 4
	 */
//	xTaskCreate(animationTask1, "LedAnimationTask1", 128, NULL, 1, NULL);
//	xTaskCreate(animationTask2, "LedAnimationTask2", 128, NULL, 1, NULL);
//	xTaskCreate(animationTask3, "LedAnimationTask2", 128, NULL, 1, NULL);
	/*
	 * EJERCICIO 5
	 */
	xTaskCreate(JoyTask, "JoyTask", 128, NULL, 1, NULL);
	/*
	 * EJERCICIO 6
	 */
	xTaskCreate(animationTask1, "LedAnimationTask1", 128, NULL, 1, &taskHandler1);
	xTaskCreate(animationTask2, "LedAnimationTask2", 128, NULL, 1, &taskHandler2);
	xTaskCreate(animationTask3, "LedAnimationTask3", 128, NULL, 1, &taskHandler3);
}

/*
 * EJERCICIO 1
 */
void LedToggleTask1(void *pArg) {
	for (;;) {
		int led = *(int*) pArg;
		LED_Toggle(led);
		vTaskDelay(1000);
	}
}

/*
 * EJERCICIO 2
 */
void LedToggleTask2(void *pArg) {
	int i = *(int*) pArg;
	for (;;) {
		switch (i) {
		case 0:
			LED_Toggle(i);
			vTaskDelay(150);
			break;
		case 1:
			LED_Toggle(i);
			vTaskDelay(100);
			break;
		case 2:
			LED_Toggle(i);
			vTaskDelay(250);
			break;
		case 3:
			LED_Toggle(i);
			vTaskDelay(500);
			break;
		}

	}
}

/*
 * EJERCICIO 3
 */
void LedToggleTask3(void *pArg) {
	int i = *(int*) pArg;
	int conmutaciones = 0;
	for (;;) {
		LED_Toggle(i);
		vTaskDelay(500);
		conmutaciones++;
		if (conmutaciones == 10) {
			conmutaciones = 0;
			i=(i==3) ? 1 : i+1;
			xTaskCreate(LedToggleTask3, "LedToogleTask4", 128, &i, 1, NULL);
			vTaskDelete(NULL);
		}
	}
}


/*
 * EJERCICIO 5
 */
void JoyTask(void *pArg) {
	for (;;) {
		uint8_t joyState = ReadJoy();
		if (joyState != 0) {
//			LED_Toggle(0);
			switch (numPulsaciones) {
			case 1:
				xTaskNotifyGive(taskHandler1);
				break;
			case 2:
				xTaskNotifyGive(taskHandler2);
				break;
			case 3:
				xTaskNotifyGive(taskHandler3);
				break;
			}

		}
		vTaskDelay(10);
	}
}

/*
 * EJERCICIO 4 y 6 #para el ej 6 se ha añadido la espera por notificacion
 */
void animationTask1(void *pArg) {
	for (;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		for (int i = 0; i < 3; i++) {
			LED_Toggle(1);
			vTaskDelay(500);
			LED_Toggle(1);
			vTaskDelay(500);
		}
	}
}

void animationTask2(void *pArg) {
	for (;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		LED_Toggle(2);
		vTaskDelay(600);
		LED_Toggle(2);
		vTaskDelay(300);

		LED_Toggle(2);
		vTaskDelay(300);
		LED_Toggle(2);
		vTaskDelay(300);

		LED_Toggle(2);
		vTaskDelay(300);
		LED_Toggle(2);
		vTaskDelay(600);
	}
}

void animationTask3(void *pArg) {
	for (;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		LED_Toggle(3);
		vTaskDelay(300);
		LED_Toggle(3);
		vTaskDelay(150);

		LED_Toggle(3);
		vTaskDelay(600);
		LED_Toggle(3);
		vTaskDelay(200);

		LED_Toggle(3);
		vTaskDelay(100);
		LED_Toggle(3);
		vTaskDelay(700);
	}
}
