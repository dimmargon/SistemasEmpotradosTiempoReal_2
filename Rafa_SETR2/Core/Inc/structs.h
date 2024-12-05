/*
 * structs.h
 *
 *  Created on: Dec 4, 2024
 *      Author: dimasPortatil
 */

#ifndef INC_STRUCTS_H_
#define INC_STRUCTS_H_

#include "stdint.h"

struct  SensorData {
	float temperature;
	float humidity;
	float pressure;
	int16_t accelData[3];
	float gyro[3];
	int16_t magXYZ[3];
};

#endif /* INC_STRUCTS_H_ */
