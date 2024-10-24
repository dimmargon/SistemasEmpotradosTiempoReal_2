#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "SerialTask.h"

extern UART_HandleTypeDef huart1;
SemaphoreHandle_t xSemaphore;
QueueHandle_t xQueue;

void CreateSerialObjects()
{
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);
    xQueue = xQueueCreate(16, sizeof(char));
}

void CreateTasks()
{
    xTaskCreate(serialRxTask, "SerialRxTask", 128, NULL, 1, NULL);

    //aqui inicio la recepcion del primer caracter
    char initialChar;
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&initialChar, 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    static signed long xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static signed long xHigherPriorityTaskWoken = pdFALSE;
    char receivedChar;

    //envio el caracter recibido a la cola
    //inicio la recepcion del siguiente caracter
    if (HAL_UART_Receive_IT(&huart1, (uint8_t*)&receivedChar, 1) == HAL_OK)
    {
        xQueueSendFromISR(xQueue, &receivedChar, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void serialRxTask(void *pvParameters)
{
    char receivedChar;

    printf("Hola, Mundo!\r\n");

    while (1)
    {
        if (xQueueReceive(xQueue, &receivedChar, portMAX_DELAY) == pdTRUE)
        {
            printf("Nuevo dato recibido: %c\r\n", receivedChar);
        }
    }
}

int __io_putchar(int ch)
{
    BaseType_t status = xSemaphoreTake(xSemaphore, portMAX_DELAY);

    if (status == pdTRUE)
    {
    	//transmito el caracter por UART
        HAL_UART_Transmit_IT(&huart1, (uint8_t*)&ch, 1);
    }
    return ch;
}

int __io_getchar(void)
{
    uint8_t ch = 0;
    //recibo el caracter
    HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);
    return ch;
}
