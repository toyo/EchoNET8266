/*
 * EOJProfile.cpp
 *
 *  Created on: 2015/10/02
 *      Author: toyo
 */

#include "EOJProfile.h"
extern "C"{
#include <c_types.h> // uint8_t
#include "espmissing.h" // ets_uart_printf
}

ICACHE_FLASH_ATTR EOJProfile::EOJProfile() {
	ets_uart_printf("EOJProfile()\n");
}

int ICACHE_FLASH_ATTR EOJProfile::getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJProfile::getGetPropertyMap\n");

		resp[0]=0x8a;
		resp[1]=0x9d;
		resp[2]=0x9e;
		resp[3]=0x9f;

		return 4;
}

int ICACHE_FLASH_ATTR EOJProfile::getSetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJProfile::getSetPropertyMap\n");

		return 0;
}

bool ICACHE_FLASH_ATTR EOJProfile::getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt){
	if(eoj1 != 0x0e){
		return false;
	}
	uint8_t resp;

	switch(epc){
	case 0x8a://getManufacturerCode(r,pdc,edt);
		ets_uart_printf("EOJProfile::getManufacturerCode\n");
		r.set(3,(uint8_t*)MANUFACTURER);
		return true;
	case 0x9d://getStatusChangeAnnouncementPropertyMap(r,pdc,edt);
		ets_uart_printf("EOJProfile::getStatusChangeAnnouncementPropertyMap\n");
		resp=0;
		r.set(1,&resp);
		return true;
	case 0x9e://getSetPropertyMap(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJProfile::getSetPropertyMap\n");
		uint8_t res[128];
		r.set((res[0]=getSetPropertyMap(res+1,pdc,edt))+1,(uint8_t*)res);
		return true;
	case 0x9f://getGetPropertyMap(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJProfile::getGetPropertyMap\n");
		r.set((res[0]=getGetPropertyMap(res+1,pdc,edt))+1,(uint8_t*)res);
		return true;
	default:
		return false;
	}
}




