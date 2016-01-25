/*
 * AirPressureSensor.h
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */

#ifndef USER_AIRPRESSURESENSOR_H_
#define USER_AIRPRESSURESENSOR_H_

/// 気圧センサークラス
class AirPressureSensor{
public:	/// 気圧の測定値 0.1hPa
	virtual int32_t measuredAirPressureValue()=0;
	virtual ~AirPressureSensor(){};
};

#endif /* USER_AIRPRESSURESENSOR_H_ */
