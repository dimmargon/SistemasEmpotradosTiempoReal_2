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
TaskHandle_t taskHandler;
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
//	xTaskCreate(LedToggleTask3v2, "LedToogleTask1", 128, &led, 1, NULL);
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
	xTaskCreate(LedAnimationTask6, "AnimationTask", 128, NULL, 1, &taskHandler);
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
//void LedToggleTask3v1(void *pArg) {
//	int i = *(int*) pArg;
//	int conmutaciones = 0;
//	for (;;) {
//		switch (i) {
//		case 1:
//			LED_Toggle(i);
//			vTaskDelay(500);
//			conmutaciones++;
//			if (conmutaciones == 10) {
//				conmutaciones = 0;
//				i = i + 1;
//				xTaskCreate(LedToggleTask3, "LedToogleTask2", 128, &i, 1, NULL);
//				vTaskDelete(NULL);
//			}
//			break;
//		case 2:
//			LED_Toggle(i);
//			vTaskDelay(500);
//			conmutaciones++;
//			if (conmutaciones == 10) {
//				conmutaciones = 0;
//				i = i + 1;
//				xTaskCreate(LedToggleTask3, "LedToogleTask3", 128, &i, 1, NULL);
//				vTaskDelete(NULL);
//			}
//			break;
//		case 3:
//			LED_Toggle(i);
//			vTaskDelay(500);
//			conmutaciones++;
//			if (conmutaciones == 10) {
//				conmutaciones = 0;
//				i = 0;
//				i = i + 1;
//				xTaskCreate(LedToggleTask3, "LedToogleTask4", 128, &i, 1, NULL);
//				vTaskDelete(NULL);
//			}
//			break;
//
//		}
//	}
//}

void LedToggleTask3v2(void *pArg) {
	int i = *(int*) pArg;
	int conmutaciones = 0;
	for (;;) {
		LED_Toggle(i);
		vTaskDelay(500);
		conmutaciones++;
		if (conmutaciones == 10) {
			conmutaciones = 0;
			i=(i==3) ? 1 : i+1;
			xTaskCreate(LedToggleTask3v2, "LedToogleTask4", 128, &i, 1, NULL);
			vTaskDelete(NULL);
		}
	}
}

/*
 * EJERCICIO 4
 */
void animationTask1(void *pArg) {
//	for(;;){
	LED_Toggle(1);
	vTaskDelay(200);
	LED_Toggle(1);
		vTaskDelay(200);
		LED_Toggle(1);
			vTaskDelay(200);
//	}
}

void animationTask2(void *pArg) {
//	for(;;){
	LED_Toggle(2);
	vTaskDelay(200);
	LED_Toggle(2);
		vTaskDelay(200);
		LED_Toggle(2);
			vTaskDelay(200);
//	}
}

void animationTask3(void *pArg) {
//	for(;;){
	LED_Toggle(3);
	vTaskDelay(200);
	LED_Toggle(3);
		vTaskDelay(200);
		LED_Toggle(3);
			vTaskDelay(200);
//	}
}

/*
 * EJERCICIO 5
 */
void JoyTask(void *pArg){
	for(;;){
		uint8_t joyState = ReadJoy();
		if (joyState != 0) {
//			LED_Toggle(0);
			xTaskNotifyGive(taskHandler);
		}
		vTaskDelay(10);
	}
}

/*
 * EJERCICIO 6
 */
void LedAnimationTask6(void *pArg) {

	for (;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		switch(numPulsaciones){
		case 1:
			animationTask1(NULL);
			break;
		case 2:
			animationTask2(NULL);
			break;
		case 3:
			animationTask3(NULL);
			break;
		}
	}
}



