/*
 * EOJGeneralLighting.cpp
 *
 *  Created on: 2015/10/02
 *      Author: toyo
 */

#include "EOJgeneralLighting.h"

extern "C"{
#include <ip_addr.h>
#include <espconn.h>
#include "espmissing.h" // ets_uart_printf
}

ICACHE_FLASH_ATTR EOJGeneralLighting::EOJGeneralLighting(uint8_t i) : instance(i),EOJDevice(false)  {// 0x02 0x90
	ets_uart_printf("EOJGeneralLighting(%d)\n",i);

}

int ICACHE_FLASH_ATTR EOJGeneralLighting::getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
	ets_uart_printf("EOJGeneralLighting::getGetPropertyMap\n");
	int n=EOJDevice::getGetPropertyMap(resp,pdc,edt);
	resp[n++]=0xb6;
	return n;
}

int ICACHE_FLASH_ATTR EOJGeneralLighting::getSetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
	ets_uart_printf("EOJGeneralLighting::getSetPropertyMap\n");
	int n=EOJDevice::getSetPropertyMap(resp,pdc,edt);
	resp[n++]=0x80;
	resp[n++]=0xb6;
	return n;
}


bool ICACHE_FLASH_ATTR EOJGeneralLighting::getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt){
	if(!(eoj1 == 0x02 && eoj2 == 0x90 && (eoj3 == instance || eoj3 == 0x00))){
		return false;
	}
	uint8_t resp;
	switch(epc){
	case 0xb6:// getLightingModeSetting(r,pdc,edt);
		ets_uart_printf("EOJGeneralLighting::getLightingModeSetting\n");
		resp=0x42;
		r.set(1,&resp);
		return true;
	}
	return EOJDevice::getRouting(r, eoj1, eoj2, eoj3, epc, pdc, edt);
}

bool ICACHE_FLASH_ATTR EOJGeneralLighting::setRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt){
	if(!(eoj1 == 0x02 && eoj2 == 0x90 && (eoj3 == instance || eoj3 == 0x00))){
		return false;
	}
	switch(epc){
	case 0x80: //setOperationStatus(pdc,edt);
		ets_uart_printf("EOJGeneralLighting::setOperationStatus\n");
		if(pdc > 0){
			switch(edt[0]){
			case 0x30:
				operating=true;
				becomesOn();
				return true;
			case 0x31:
				operating=false;
				becomesOff();
				return true;
			}
		}
		break;
	case 0xb6://setLightingModeSetting(pdc,edt);
		ets_uart_printf("EOJGeneralLighting::setLightingModeSetting\n");
		if(pdc > 0 && edt[0] == 0x42){
			return true;
		}
		break;
	}
	return EOJDevice::setRouting(eoj1, eoj2, eoj3, epc, pdc, edt);
}






