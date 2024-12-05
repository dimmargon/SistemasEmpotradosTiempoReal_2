/*
 * Wifi.c
 *
 *  Created on: Nov 29, 2020
 *      Author: rpaz
 */

#include <unistd.h>
#include "main.h"
#include "es_wifi.h"
#include "wifi.h"
#include "cmsis_os.h"
#include "semphr.h"

#include "structs.h"

struct SensorData GetSensores();

extern float temperature;

/* Update SSID and PASSWORD with own Access point settings */
#define SSID     "DimasMovil"
#define PASSWORD "quemiras"
#define PORT           80

#define TERMINAL_USE

#define WIFI_WRITE_TIMEOUT 10000
#define WIFI_READ_TIMEOUT  10000
#define SOCKET                 0


#ifdef  TERMINAL_USE
#define LOG(a) printf a
#else
#define LOG(a)
#endif
/* Private defines -----------------------------------------------------------*/


static  uint8_t http[1024];
static  uint8_t  IP_Addr[4];


// Prototipos
int wifi_server(void);

static  WIFI_Status_t SendWebPage(uint8_t ledIsOn, struct SensorData dim);
static  int wifi_start(void);
static  int wifi_connect(void);
static  bool WebServerProcess(void);





static int wifi_start(void)
{
  uint8_t  MAC_Addr[6];

 /*Initialize and use WIFI module */
  if(WIFI_Init() ==  WIFI_STATUS_OK)
  {
    LOG(("ES-WIFI Initialized.\n"));
    if(WIFI_GetMAC_Address(MAC_Addr) == WIFI_STATUS_OK)
    {
      LOG(("> es-wifi module MAC Address : %X:%X:%X:%X:%X:%X\n",
               MAC_Addr[0],
               MAC_Addr[1],
               MAC_Addr[2],
               MAC_Addr[3],
               MAC_Addr[4],
               MAC_Addr[5]));
    }
    else
    {
      LOG(("> ERROR : CANNOT get MAC address\n"));
      return -1;
    }
  }
  else
  {
    return -1;
  }
  return 0;
}



int wifi_connect(void)
{

  wifi_start();

  LOG(("\nConnecting to %s , %s\n",SSID,PASSWORD));
  if( WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
  {
    if(WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK)
    {
      LOG(("> es-wifi module connected: got IP Address : %d.%d.%d.%d\n",
               IP_Addr[0],
               IP_Addr[1],
               IP_Addr[2],
               IP_Addr[3]));
    }
    else
    {
		  LOG((" ERROR : es-wifi module CANNOT get IP address\n"));
      return -1;
    }
  }
  else
  {
		 LOG(("ERROR : es-wifi module NOT connected\n"));
     return -1;
  }
  return 0;
}

int wifi_server(void)
{
  bool StopServer = false;

  LOG(("\nRunning HTML Server test\n"));
  if (wifi_connect()!=0) return -1;


  if (WIFI_STATUS_OK!=WIFI_StartServer(SOCKET, WIFI_TCP_PROTOCOL, 1, "", PORT))
  {
    LOG(("ERROR: Cannot start server.\n"));
  }

  LOG(("Server is running and waiting for an HTTP  Client connection to %d.%d.%d.%d\n",IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]));

  do
  {
    uint8_t RemoteIP[4];
    uint16_t RemotePort;


    while (WIFI_STATUS_OK != WIFI_WaitServerConnection(SOCKET,1000,RemoteIP,&RemotePort))
    {
        LOG(("Waiting connection to  %d.%d.%d.%d\n",IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]));

    }

    LOG(("Client connected %d.%d.%d.%d:%d\n",RemoteIP[0],RemoteIP[1],RemoteIP[2],RemoteIP[3],RemotePort));

    StopServer=WebServerProcess();

    if(WIFI_CloseServerConnection(SOCKET) != WIFI_STATUS_OK)
    {
      LOG(("ERROR: failed to close current Server connection\n"));
      return -1;
    }
  }
  while(StopServer == false);

  if (WIFI_STATUS_OK!=WIFI_StopServer(SOCKET))
  {
    LOG(("ERROR: Cannot stop server.\n"));
  }

  LOG(("Server is stop\n"));
  return 0;
}


static bool WebServerProcess(void)
{
	uint8_t LedState = 1;
	    struct SensorData datosRecibidos;
	    uint16_t respLen;
	    static uint8_t resp[1024];
	    bool stopserver = false;

	    if (WIFI_STATUS_OK == WIFI_ReceiveData(SOCKET, resp, 1000, &respLen, WIFI_READ_TIMEOUT)) {
	        LOG(("get %d bytes from server\n", respLen));

	        if (respLen > 0) {
	            if (strstr((char *)resp, "GET")) { /* GET: send JSON data */
	                datosRecibidos = GetSensores();

	                // Generar y enviar JSON
	                if (SendWebPage(LedState, datosRecibidos) != WIFI_STATUS_OK) {
	                    LOG(("> ERROR: Cannot send JSON response\n"));
	                }
	                else {
	                    LOG(("Sent JSON response after GET command\n"));
	                }
	            } else if (strstr((char *)resp, "POST")) { /* POST: process received info */
	                LOG(("Post request\n"));

	                if (strstr((char *)resp, "radio")) {
	                    if (strstr((char *)resp, "radio=0")) {
	                        LedState = 0;
	                        EncolarLED(LedState);
	                    } else if (strstr((char *)resp, "radio=1")) {
	                        LedState = 1;
	                        EncolarLED(LedState);
	                    }
	                }
	                if (strstr((char *)resp, "stop_server")) {
	                    if (strstr((char *)resp, "stop_server=0")) {
	                        stopserver = false;
	                    } else if (strstr((char *)resp, "stop_server=1")) {
	                        stopserver = true;
	                    }
	                }

	                datosRecibidos = GetSensores();

	                // Generar y enviar JSON
	                if (SendWebPage(LedState, datosRecibidos) != WIFI_STATUS_OK) {
	                    LOG(("> ERROR: Cannot send JSON response\n"));
	                }
	                else {
	                    LOG(("Sent JSON response after POST command\n"));
	                }
	            }
	        }
	    }
	    else LOG(("Client closed connection\n"));
	    return stopserver;

 }

/**
  * @brief  Send HTML page
  * @param  None
  * @retval None
  */
static WIFI_Status_t SendWebPage(uint8_t ledIsOn, struct SensorData dim)
{
	uint8_t character[50];

	  uint16_t SentDataLength;
	  WIFI_Status_t ret;

	  /* Construct web page content */
	  strcpy((char *)http, (char *)"HTTP/1.0 200 OK\r\nContent-Type: application/json\r\nPragma: no-cache\r\n\r\n");
	  strcat((char *)http, (char *)"{");

	  strcat((char *)http, (char *)"\"Sensores\": {");
	  strcat((char *)http, (char *)"\"temperature\":");
	  sprintf((char *)character, "%.1f", dim.temperature);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)",\"humidity\":");
	  sprintf((char *)character, "%.1f", dim.humidity);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)",\"pressure\":");
	  sprintf((char *)character, "%.1f", dim.pressure);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)"},");

	  strcat((char *)http, (char *)"\"Accelleration\": {");
	  strcat((char *)http, (char *)"\"acc_x\":");
	  sprintf((char *)character, "%d", (int)dim.accelData[0]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)",\"acc_y\":");
	  sprintf((char *)character, "%d", (int)dim.accelData[1]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)",\"acc_z\":");
	  sprintf((char *)character, "%d", (int)dim.accelData[2]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)"},");

	  strcat((char *)http, (char *)"\"Gyroscope\": {");
	  strcat((char *)http, (char *)"\"gyro_x\":");
	  sprintf((char *)character, "%.1f", (float)dim.gyro[0]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)",\"gyro_y\":");
	  sprintf((char *)character, "%.1f", (float)dim.gyro[1]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)",\"gyro_z\":");
	  sprintf((char *)character, "%.1f", (float)dim.gyro[2]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)"},");

	  strcat((char *)http, (char *)"\"Magnetometer\": {");
	  strcat((char *)http, (char *)"\"mag_x\":");
	  sprintf((char *)character, "%d", (int)dim.magXYZ[0]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)",\"mag_y\":");
	  sprintf((char *)character, "%d", (int)dim.magXYZ[1]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)",\"mag_z\":");
	  sprintf((char *)character, "%d", (int)dim.magXYZ[2]);
	  strcat((char *)http, (char *)character);
	  strcat((char *)http, (char *)"}");

	  strcat((char *)http, (char *)"}");
	/*
	  if (ledIsOn) {
		strcat((char *)http, (char *)"{");
	    strcat((char *)http, (char *)"<p><input type=\"radio\" name=\"radio\" value=\"0\" >LED off");
	    strcat((char *)http, (char *)"<br><input type=\"radio\" name=\"radio\" value=\"1\" checked>LED on");
	  }
	  else {
	    strcat((char *)http, (char *)"<p><input type=\"radio\" name=\"radio\" value=\"0\" checked>LED off");
	    strcat((char *)http, (char *)"<br><input type=\"radio\" name=\"radio\" value=\"1\" >LED on");
	  }
	*/
	  //strcat((char *)http, (char *)"</strong><p><input type=\"submit\"></form></span>");
	  //strcat((char *)http, (char *)"</body>\r\n</html>\r\n");

	  ret = WIFI_SendData(0, (uint8_t *)http, strlen((char *)http), &SentDataLength, WIFI_WRITE_TIMEOUT);

	  if((ret == WIFI_STATUS_OK) && (SentDataLength != strlen((char *)http))) {
	    ret = WIFI_STATUS_ERROR;
	  }

	  return ret;
}


//*************************************************************************//

extern SPI_HandleTypeDef hspi3;

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file.                                                                     */
/************************************************************
 *															*
 *		Rutinas de servicio de interrupciones				*
 *															*
 ************************************************************/

/**
  * @brief  This function handles external lines 1interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
 HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
 portYIELD_FROM_ISR(pdFALSE);
}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case (GPIO_PIN_1):
    {
      SPI_WIFI_ISR();
      break;
    }
    default:
    {
      break;
    }
  }
  portYIELD_FROM_ISR(pdFALSE);
}

/**
  * @brief  SPI3 line detection callback.
  * @param  None
  * @retval None
  */
extern  SPI_HandleTypeDef hspi;
void SPI3_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hspi);
  portYIELD_FROM_ISR(pdFALSE);
}
