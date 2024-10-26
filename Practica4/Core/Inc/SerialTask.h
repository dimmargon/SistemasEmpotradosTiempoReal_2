/*
 * SerialTask.h
 *
 *  Created on: Oct 24, 2024
 *      Author: dimasPortatil
 */

#ifndef INC_SERIALTASK_H_
#define INC_SERIALTASK_H_

#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stdio.h"

extern SemaphoreHandle_t xSemaphore;
extern QueueHandle_t xQueue;

void CreateSerialObjects(void);
void CreateTasks(void);
void serialRxTask(void *pvParameters);

#endif /* INC_SERIALTASK_H_ */
