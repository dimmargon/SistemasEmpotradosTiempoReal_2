#include "SerialTask.h"

extern UART_HandleTypeDef huart1;
SemaphoreHandle_t xSemaphore;
QueueHandle_t xQueue;

char receivedChar; //buffer para el caracter

void CreateSerialObjects()
{
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore); //disponible inicialmente
    xQueue = xQueueCreate(16, sizeof(char)); //almaceno hasta 16 caracteres
}

void CreateTasks()
{
    xTaskCreate(serialRxTask, "SerialRxTask", 128, NULL, 1, NULL);

    //inicio la recepcion del primer caracter
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&receivedChar, 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    static signed long xHigherPriorityTaskWoken = pdFALSE;
    //libero el semaforo
    xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); //solo en caso de prioridades mayores
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static signed long xHigherPriorityTaskWoken = pdFALSE;
    //envio el caracter recibido a la cola
    xQueueSendFromISR(xQueue, &receivedChar, &xHigherPriorityTaskWoken);

    //inicio la recepcion del siguiente caracter
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&receivedChar, 1);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); //solo en caso de prioridades mayores
}

void serialRxTask(void *pvParameters)
{
    char receivedChar;

    printf("Hola, Mundo!\r\n");

    while (1)
    {
    	//espero hasta que haya un caracter en la cola
        if (xQueueReceive(xQueue, &receivedChar, portMAX_DELAY) == pdTRUE)
        {
            printf("Nuevo dato recibido: %c\r\n", receivedChar);
        }
    }
}

int __io_putchar(int ch)
{
    BaseType_t status = xSemaphoreTake(xSemaphore, portMAX_DELAY);

    if (status == pdTRUE) //espero al semaforo
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
