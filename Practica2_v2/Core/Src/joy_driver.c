/*
 * joy_driver.c
 *
 *  Created on: Sep 19, 2024
 *      Author: dimasPortatil
 */
#include "stm32l4xx_hal.h"

//uint8_t numPulsaciones;
//
//uint8_t ReadJoy(void){
//	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET){
//		numPulsaciones;
//		return 1;
//	}else{
//		return 0;
//	}
//
//}

uint8_t numPulsaciones = 0;
uint8_t estadoAnterior = 1;  // 1 significa que no está presionado (GPIO_PIN_SET)

uint8_t ReadJoy(void){
    uint8_t estadoActual = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

    // Detectar flanco de bajada (cuando pasa de no presionado a presionado)
    if(estadoAnterior == GPIO_PIN_SET && estadoActual == GPIO_PIN_RESET){
        numPulsaciones++;  // Incrementar el contador de pulsaciones
        if(numPulsaciones==4) numPulsaciones=1;
    }

    // Actualizar el estado anterior para la próxima lectura
    estadoAnterior = estadoActual;

    // Devolver 1 si el botón está presionado, 0 si no lo está
    if (estadoActual == GPIO_PIN_RESET) {
        return 1;
    } else {
        return 0;
    }
}
