/*
 * EOJTemperatureSensor.h
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */

#ifndef USER_EOJTEMPERATURESENSOR_H_
#define USER_EOJTEMPERATURESENSOR_H_

#include "EOJDevice.h"
#include "TemperatureSensor.h"

/// Echonet Object for Temperature Sensor.
class EOJTemperatureSensor : public EOJDevice{ // 0x02 0x90
	TemperatureSensor* t;
protected:
	uint8_t instance;
	int getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
public:
	EOJTemperatureSensor(TemperatureSensor* temp,uint8_t i=0x01);
	virtual ~EOJTemperatureSensor(){
	}
	bool getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);

	uint8_t eoj1(){
		return 0x00;
	}
	uint8_t eoj2(){
		return 0x11;
	}
	uint8_t eoj3(){
		return instance;
	}
};

#endif /* USER_EOJTEMPERATURESENSOR_H_ */
