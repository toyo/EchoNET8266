/**
 * @file EOJDevice.cpp
 * @brief EOJDevice data implementation.
 * @author toyo
 * @date 2015/10/02
 */

#include "EOJDevice.h"
extern "C"{
#include <c_types.h> // uint8_t
#include "espmissing.h" // ets_uart_printf
}

const uint8_t getStandardVersionInformation[4]={0x00,0x00,'B',0x00};

ICACHE_FLASH_ATTR EOJDevice::EOJDevice(bool op,uint8_t loc) :  operating(op), location(loc), fault(false){
	ets_uart_printf("EOJDevice()\n");
}

int ICACHE_FLASH_ATTR EOJDevice::getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getGetPropertyMap\n");
		resp[0]=0x80;
		resp[1]=0x81;
		resp[2]=0x82;
		resp[3]=0x88;
		resp[4]=0x8a;
		resp[5]=0x9d;
		resp[6]=0x9e;
		resp[7]=0x9f;

		return 8;
}

int  ICACHE_FLASH_ATTR EOJDevice::getSetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getSetPropertyMap\n");
		resp[0]=0x81;

		return 1;
}

bool ICACHE_FLASH_ATTR EOJDevice::getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt){
	if( eoj1 > 0x06 ){
		return false;
	}
	uint8_t resp;

	switch(epc){
	case 0x80://getOperationStatus(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getOperationStatus\n");
		resp=operating?0x30:0x31;
		r.set(1,&resp);
		return true;
	case 0x81://getInstallationLocation(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getInstallationLocation\n");
		resp=location;
		r.set(1,&resp);
		return true;
	case 0x82://getStandardVersionInformation(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getStandardVersionInformation\n");
		r.set(sizeof(getStandardVersionInformation),getStandardVersionInformation);
		return true;
	case 0x88://getFaultStatus(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getFaultStatus\n");
		resp=fault?0x41:0x42;
		r.set(1,&resp);
		return true;
	case 0x8a://getManufacturerCode(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getManufacturerCode\n");
		r.set(3,(uint8_t*)MANUFACTURER);
		return true;
	case 0x9b://getSetMPropertyMap(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getSetMPropertyMap\n");
		resp=0;
		r.set(1,&resp);
		return true;
	case 0x9c://getGetMPropertyMap(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getGetMPropertyMap\n");
		resp=0;
		r.set(1,&resp);
		return true;
	case 0x9d://getStatusChangeAnnouncementPropertyMap(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getStatusChangeAnnouncementPropertyMap\n");
		resp=0;
		r.set(1,&resp);
		return true;
	case 0x9e://getSetPropertyMap(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getSetPropertyMap\n");
		uint8_t res[128];
		r.set((res[0]=getSetPropertyMap(res+1,pdc,edt))+1,(uint8_t*)res);
		return true;
	case 0x9f://getGetPropertyMap(Response& r,uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::getGetPropertyMap\n");
		r.set((res[0]=getGetPropertyMap(res+1,pdc,edt))+1,(uint8_t*)res);
		return true;
	default:
		return false;
	}
}

bool ICACHE_FLASH_ATTR EOJDevice::setRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt){
	switch(epc){
	case 0x81://setInstallationLocation(uint8_t pdc,uint8_t* edt){
		ets_uart_printf("EOJDevice::setInstallationLocation\n");
		if(pdc != 1) return false;
		location=edt[0];
		return true;
	default:
		return false;
	}
}

void ICACHE_FLASH_ATTR EOJDevice::annoRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3){
}

