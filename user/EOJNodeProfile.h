/*
 * EOJNodeProfile.h
 *
 *  Created on: 2015/10/02
 *      Author: toyo
 */

#ifndef EOJNODEPROFILE_H_
#define EOJNODEPROFILE_H_

#include "EOJProfile.h"
#include "EOJDevice.h"


class EOJNodeProfile : protected EOJProfile{ // 0x0e 0xf0 0x01
	EOJDevice* d[50];
	int numOfDevices; // max 50
	uint8_t deviceClasses[20][2];
	uint8_t numOfDeviceClasses; // max 20

protected:
	uint8_t instance;
	int getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
	int getSetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
public:
	bool myDEOJ(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3){
		if (eoj1 == 0x0e && eoj2 == 0xf0 && ( eoj3 == instance || eoj3 == 0)) return true;
		for(int i=0;i<numOfDevices;i++)
			if (d[i]->myDEOJ(eoj1,eoj2,eoj3)) return true;
		return false;
	}
	bool getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);
	bool setRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);
	void annoRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3);

	EOJNodeProfile(EOJDevice* dev,uint8_t i=1);
	void addDevice(EOJDevice* dev){
		int i;
		for(i=0;i<numOfDeviceClasses;i++){
			if(deviceClasses[i][0] == dev->eoj1() && deviceClasses[i][1] == dev->eoj2()) break;
		}
		if(i == numOfDeviceClasses){ // non exist
			deviceClasses[numOfDeviceClasses][0]=d[0]->eoj1();
			deviceClasses[numOfDeviceClasses][1]=d[0]->eoj2();
			numOfDeviceClasses++;
		}
		d[numOfDevices++]=dev;
	}
};

#endif /* EOJNODEPROFILE_H_ */
