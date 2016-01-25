/*
 * BMP180.cpp
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */

extern "C"{
#include "driver/i2c.h"
#include "driver/i2c_bmp180.h"
#include "espmissing.h" // ets_uart_printf
}

#include "BMP180.h"
ICACHE_FLASH_ATTR BMP180::BMP180() {
	if (BMP180_Init())
		ets_uart_printf("BMP180 init done.\r\n");
	else
		ets_uart_printf("BMP180 init error.\r\n");
}

int32_t ICACHE_FLASH_ATTR BMP180::measuredTemperatureValue(){
	return BMP180_GetTemperature();
}

int32_t ICACHE_FLASH_ATTR BMP180::measuredAirPressureValue(){
	return BMP180_GetPressure(OSS_0)/10;
}

