/*
 * TemperatureSensor.h
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */

#ifndef USER_TEMPERATURESENSOR_H_
#define USER_TEMPERATURESENSOR_H_

class TemperatureSensor{
public:
	/// 気温の測定値 0.1℃
	virtual int32_t measuredTemperatureValue()=0;
	virtual ~TemperatureSensor(){};
};
#endif /* USER_TEMPERATURESENSOR_H_ */
