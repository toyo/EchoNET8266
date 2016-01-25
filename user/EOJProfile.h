/*
 * EOJProfile.h
 *
 *  Created on: 2015/10/02
 *      Author: toyo
 */

#ifndef EOJPROFILE_H_
#define EOJPROFILE_H_

#include "Response.h"

class EOJProfile { // 0x0e
protected:
	int getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
	int getSetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
public:
	virtual bool myDEOJ(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3){
		return eoj1 == 0x0e;
	}
	virtual bool getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);
	virtual bool setRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt){return false;};
	virtual void annoRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3){};
	EOJProfile();
	virtual ~EOJProfile(){}
};

#endif /* EOJPROFILE_H_ */
