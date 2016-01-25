/*
 * EOJTemperatureSensor.cpp
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */

#include "EOJTemperatureSensor.h"

extern "C"{
#include "driver/i2c.h"
#include "driver/i2c_bmp180.h"
#include "espmissing.h" // ets_uart_printf
}


ICACHE_FLASH_ATTR EOJTemperatureSensor::EOJTemperatureSensor(TemperatureSensor* temp,uint8_t i) : t(temp),instance(i) {// 0x00 0x11
	ets_uart_printf("EOJTemperatureSensor(%02d%02d%02d)\n",eoj1(),eoj2(),eoj3());
	instance=i;
	operating=false;
}

int ICACHE_FLASH_ATTR EOJTemperatureSensor::getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJTemperatureSensor::getGetPropertyMap\n");
		int n=EOJDevice::getGetPropertyMap(resp,pdc,edt);
		resp[n++]=0xe0;
		return n;
}

bool ICACHE_FLASH_ATTR EOJTemperatureSensor::getRouting(Response& r,uint8_t e1,uint8_t e2,uint8_t e3,uint8_t epc,uint8_t pdc,uint8_t* edt){
	if(!(e1 == eoj1() && e2 == eoj2() && (e3 == instance || e3 == 0x00))){
		return false;
	}

	switch(epc){
	case 0xe0:// getMeasuredTemperatureValue
		int16_t temp=t->measuredTemperatureValue(); // 10 times
		uint8_t resp[2];
		resp[0]=temp >> 8;
		resp[1]=temp & 0xff;
		ets_uart_printf("EOJTemperatureSensor::getMeasuredTemperatureValue = %d\n",temp);
		r.set(2,(uint8_t*)resp);
		return true;
	}
	return EOJDevice::getRouting(r, e1, e2, e3, epc, pdc, edt);
}
