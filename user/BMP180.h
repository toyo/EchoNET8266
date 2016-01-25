/*
 * BMP180.h
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */

#ifndef USER_BMP180_H_
#define USER_BMP180_H_
#include "TemperatureSensor.h"
#include "AirPressureSensor.h"

/// class for BMP180
class BMP180 : public TemperatureSensor, public AirPressureSensor {
public:
	int32_t  measuredTemperatureValue();
	int32_t  measuredAirPressureValue();

public:
	BMP180();
	virtual ~BMP180(){
	}
};




#endif /* USER_BMP180_H_ */
