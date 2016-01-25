/*
 * EOJNodeProfile.cpp
 *
 *  Created on: 2015/10/02
 *      Author: toyo
 */
#include <user_interface.h>
#include <osapi.h> // ets_uart_printf

extern "C"{
#include <espconn.h>// udp send
#include "espmissing.h" // ets_uart_printf
}

#include "EOJNodeProfile.h"
#include "config.h"
#include "driver/uart.h"

const uint8_t respVersionInformation[4]={1,11,1,0};// 1.11 , Only for Specified message format
const uint8_t respIdentificationNumber[18]=FEMANUFACTURERUNIQUEID;// 1.11 , Only for Specified message format

ICACHE_FLASH_ATTR EOJNodeProfile::EOJNodeProfile(EOJDevice* dev,uint8_t i) : EOJProfile(),instance(i){
	ets_uart_printf("EOJNodeProfile(%d)\n",i);
	d[0]=dev;
	numOfDevices=1;
	numOfDeviceClasses=1;
	deviceClasses[0][0]=d[0]->eoj1();
	deviceClasses[0][1]=d[0]->eoj2();
}



int ICACHE_FLASH_ATTR EOJNodeProfile::getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
	ets_uart_printf("EOJGeneralLighting::getGetPropertyMap\n");
	int n=EOJProfile::getGetPropertyMap(resp,pdc,edt);
	resp[n++]=0x80;
	resp[n++]=0x82;
	resp[n++]=0x83;
	resp[n++]=0xd3;
	resp[n++]=0xd4;
	resp[n++]=0xd5;
	resp[n++]=0xd6;
	resp[n++]=0xd7;

	return n;
}

int ICACHE_FLASH_ATTR EOJNodeProfile::getSetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt){
	ets_uart_printf("EOJGeneralLighting::getSetPropertyMap\n");
	int n=EOJProfile::getSetPropertyMap(resp,pdc,edt);
	resp[n++]=0x80;

	return n;
}

bool ICACHE_FLASH_ATTR EOJNodeProfile::getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt){
	ets_uart_printf("EOJNodeProfile::getRouting %02x%02x%02x %02x %02x\n",eoj1,eoj2,eoj3,epc,pdc);

	for (int i=0;i<numOfDevices ; i++)
		if(d[i]->myDEOJ(eoj1,eoj2,eoj3))
			return d[i]->getRouting(r,eoj1,eoj2,eoj3,epc,pdc,edt);

	if(eoj1==0x0e && eoj2==0xf0 && (eoj3 == 0x01 || eoj3 == 0)){
		uint8_t resp;

		switch(epc){
		case 0x80://getOperatingStatus
			ets_uart_printf("EOJNodeProfile::getOperatingStatus\n");
			resp=0x30;// edt Booting
			r.set(1,&resp);
			return true;
		case 0x82://getVersionInformation
			ets_uart_printf("EOJNodeProfile::getVersionInformation\n");
			r.set(sizeof(respVersionInformation),respVersionInformation);
			return true;
		case 0x83://getIdentificationNumber // 6.11.1
			ets_uart_printf("EOJNodeProfile::getIdentificationNumber\n");
			r.set(17,respIdentificationNumber);
			return true;
		case 0xd3://getNumberOfSelfNodeInstances
			uint8_t respNumberOfSelfNodeInstances[3];
			ets_uart_printf("EOJNodeProfile::getNumberOfSelfNodeInstances\n");
			respNumberOfSelfNodeInstances[0]=(numOfDevices >> 16) && 0xff;
			respNumberOfSelfNodeInstances[1]=(numOfDevices >> 8) && 0xff;
			respNumberOfSelfNodeInstances[2]= numOfDevices && 0xff;
			r.set(sizeof(respNumberOfSelfNodeInstances),respNumberOfSelfNodeInstances);
			return true;
		case 0xd4://getNumberOfSelfNodeClasses
			uint8_t respNumberOfSelfNodeClasses[2];
			ets_uart_printf("EOJNodeProfile::getNumberOfSelfNodeClasses\n");
			respNumberOfSelfNodeClasses[0]=((numOfDeviceClasses+1) >> 8) && 0xff;
			respNumberOfSelfNodeClasses[1]= (numOfDeviceClasses+1) && 0xff;
			r.set(sizeof(respNumberOfSelfNodeClasses),respNumberOfSelfNodeClasses);
			return true;
		case 0xd5://getInstanceListNotification // anno
		{
			uint8_t* respInstanceListNotification = new uint8_t[numOfDevices*3+1];//0x029001 = general lighting instance
			ets_uart_printf("EOJNodeProfile::getInstanceListNotification\n");
			respInstanceListNotification[0]=numOfDevices;
			for(int i=0;i<numOfDevices;i++){
				respInstanceListNotification[i*3+1]=d[i]->eoj1();
				respInstanceListNotification[i*3+2]=d[i]->eoj2();
				respInstanceListNotification[i*3+3]=d[i]->eoj3();
			}
			r.set(numOfDevices*3+1,respInstanceListNotification);
			delete[] respInstanceListNotification;
			return true;
		}
		case 0xd6://getSelfNodeInstanceListS
		{
			ets_uart_printf("EOJNodeProfile::getSelfNodeInstanceListS\n");

			uint8_t* respSelfNodeInstanceListS = new uint8_t[numOfDevices*3+1];//0x029001 = general lighting instance
			respSelfNodeInstanceListS[0]=numOfDevices;
			for(int i=0;i<numOfDevices;i++){
				respSelfNodeInstanceListS[i*3+1]=d[i]->eoj1();
				respSelfNodeInstanceListS[i*3+2]=d[i]->eoj2();
				respSelfNodeInstanceListS[i*3+3]=d[i]->eoj3();
			}
			r.set(numOfDevices*3+1,respSelfNodeInstanceListS);
			return true;
		}
		case 0xd7://getSelfNodeClassListS
			uint8_t respSelfNodeClassListS[sizeof(deviceClasses)+1];//0x0290 = general lighting class
			ets_uart_printf("EOJNodeProfile::getSelfNodeClassListS\n");
			respSelfNodeClassListS[0]=numOfDeviceClasses;
			os_memcpy(respSelfNodeClassListS+1,deviceClasses,sizeof(deviceClasses));
			r.set((numOfDeviceClasses)*2+1,respSelfNodeClassListS);
			return true;
		}
	}
	return EOJProfile::getRouting(r, eoj1, eoj2, eoj3, epc, pdc, edt);
}

bool ICACHE_FLASH_ATTR EOJNodeProfile::setRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt){
	ets_uart_printf("EOJNodeProfile::setRouting %02x%02x%02x %02x %02x\n",eoj1,eoj2,eoj3,epc,pdc);

	for (int i=0;i<numOfDevices ; i++)
		if(d[i]->setRouting(eoj1,eoj2,eoj3,epc,pdc,edt)) return true;
	if(eoj1==0x0e && eoj2==0xf0 && (eoj3 == 0x01 || eoj3 == 0)){
		switch(epc){
		case 0x80://setOperatingStatus
			ets_uart_printf("EOJNodeProfile::setOperatingStatus\n"); // prints !!!Hello World!!!
			return true;
		}
	}
	return EOJProfile::setRouting(eoj1, eoj2, eoj3, epc, pdc, edt);
}

extern struct espconn *pUdpServer;

void ICACHE_FLASH_ATTR EOJNodeProfile::annoRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3){

	uint8_t resp[1024]; // anno
	uint8_t* resp_p=resp;

	*(resp_p++)=0x10;*(resp_p++)=0x81;
	*(resp_p++)=0x00;*(resp_p++)=0x00;
	*(resp_p++)=0x0e;*(resp_p++)=0xf0;*(resp_p++)=0x01;
	*(resp_p++)=0x0e;*(resp_p++)=0xf0;*(resp_p++)=0x01;

	*(resp_p++)=0x73;
	*(resp_p++)=0x01;
	*(resp_p++)=0xd5;

	*(resp_p++)=numOfDevices*3+1;
	*(resp_p++)=numOfDevices;


	for(int i=0;i<numOfDevices;i++){
		*(resp_p++)=d[i]->eoj1();
		*(resp_p++)=d[i]->eoj2();
		*(resp_p++)=d[i]->eoj3();
	}

	for(uint8_t* k=resp;k<resp_p;k++){
		ets_uart_printf("%02x ",*k);
	}
	ets_uart_printf("\n");

	pUdpServer->proto.udp->remote_port=3610;
	pUdpServer->proto.udp->remote_ip[0]=224;
	pUdpServer->proto.udp->remote_ip[1]=0;
	pUdpServer->proto.udp->remote_ip[2]=23;
	pUdpServer->proto.udp->remote_ip[3]=0;

	if(!espconn_send(pUdpServer,resp,resp_p-resp)){
		ets_uart_printf("%d.%d.%d.%d:%d %d.%d.%d.%d:%d\n",
				pUdpServer->proto.udp->local_ip[0],
				pUdpServer->proto.udp->local_ip[1],
				pUdpServer->proto.udp->local_ip[2],
				pUdpServer->proto.udp->local_ip[3],

				pUdpServer->proto.udp->local_port,

				pUdpServer->proto.udp->remote_ip[0],
				pUdpServer->proto.udp->remote_ip[1],
				pUdpServer->proto.udp->remote_ip[2],
				pUdpServer->proto.udp->remote_ip[3],

				pUdpServer->proto.udp->remote_port);
		ets_uart_printf("SENT!\n"); // prints !!!Hello World!!!
	}
	else{
		ets_uart_printf("SENT err!\n"); // prints !!!Hello World!!!
	}
	d[0]->annoRouting(eoj1,eoj2,eoj3);
	return;
}



