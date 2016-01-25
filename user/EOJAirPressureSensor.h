/*
 * EOJAirPressureSensor.h
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */

#ifndef USER_EOJAIRPRESSURESENSOR_H_
#define USER_EOJAIRPRESSURESENSOR_H_

#include "EOJDevice.h"
#include "AirPressureSensor.h"

/// Echonet Object for Air Pressure Sensor.
class EOJAirPressureSensor : public EOJDevice{ // 0x00 0x2d
	AirPressureSensor* s;
protected:
	uint8_t instance;
	int getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
public:
	EOJAirPressureSensor(AirPressureSensor* sensor,uint8_t i=0x01);
	virtual ~EOJAirPressureSensor(){
	}
	bool getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);

	uint8_t eoj1(){
		return 0x00;
	}
	uint8_t eoj2(){
		return 0x2d;
	}
	uint8_t eoj3(){
		return instance;
	}
};





#endif /* USER_EOJAIRPRESSURESENSOR_H_ */
