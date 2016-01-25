/*
 * EOJGeneralLighting.h
 *
 *  Created on: 2015/10/02
 *      Author: toyo
 */

#ifndef EOJGENERALLIGHTING_H_
#define EOJGENERALLIGHTING_H_

#include "EOJDevice.h"

class EOJGeneralLighting : public EOJDevice{ // 0x02 0x90

protected:
	uint8_t instance;
	int getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
	int getSetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
	virtual void becomesOn()=0;
	virtual void becomesOff()=0;

public:
	EOJGeneralLighting(uint8_t i=0x01);
	virtual ~EOJGeneralLighting(){
	}
	bool getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);
	bool setRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);

	uint8_t eoj1(){
		return 0x02;
	}
	uint8_t eoj2(){
		return 0x90;
	}
	uint8_t eoj3(){
		return instance;
	}



};




#endif /* EOJGENERALLIGHTING_H_ */
