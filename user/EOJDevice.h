/**
 * @file EOJDevice.h
 * @brief EOJDevice data header.
 * @author toyo
 * @date 2015/10/02
 */

#ifndef EOJDEVICE_H_
#define EOJDEVICE_H_

#include "Response.h"

class EOJDevice {
	uint8_t location;
	bool fault;

protected:
	bool operating;


	virtual int getGetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);
	virtual int getSetPropertyMap(uint8_t *resp,uint8_t pdc,uint8_t* edt);

public:
	EOJDevice(bool op=true,uint8_t loc=0x00);
	virtual ~EOJDevice(){
	}

	bool myDEOJ(uint8_t e1,uint8_t e2,uint8_t e3){
		return e1 == eoj1() && e2 == eoj2() && ( e3 == eoj3() || e3 == 0);
	}

	virtual bool getRouting(Response& r,uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);
	virtual bool setRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3,uint8_t epc,uint8_t pdc,uint8_t* edt);
	virtual void annoRouting(uint8_t eoj1,uint8_t eoj2,uint8_t eoj3);
	virtual uint8_t eoj1()=0;
	virtual uint8_t eoj2()=0;
	virtual uint8_t eoj3()=0;
};

#endif /* EOJDEVICE_H_ */
