/**
 * @file Response.h
 * @brief Response data header.
 * @author toyo
 * @date 2015/10/02
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "config.h"
#include <c_types.h>

/** @def
 * MAXEDTLEN is for Response.h
 */
#define MAXEDTLEN 256

#pragma pack(push,1)
/// Echonet Header structure
typedef struct _EchoNetHeader_t{
uint8_t ehd1;
uint8_t ehd2;
uint16_t tid;
uint8_t sourceclassgroup;
uint8_t sourceclass;
uint8_t sourceinstance;
uint8_t destinationclassgroup;
uint8_t destinationclass;
uint8_t destinationinstance;
uint8_t esv;
} echoNetHeader;

/// Echonet Data structure
typedef struct _EchoNetData_t{
	uint8_t epc;
	uint8_t pdc;
	uint8_t edt[MAXEDTLEN];
} echoNetData;
#pragma pack(pop)

/// Echonet response data class
class Response{
	uint8_t pdc;
	uint8_t edt[MAXEDTLEN];
public:
	Response():pdc(0){
	}
	void set(uint8_t pdc_arg,const uint8_t* edt_arg);
	uint8_t getPdc(){
		return pdc;
	}
	uint8_t* getEdt(){
		return edt;
	}
};


#endif /* RESPONSE_H_ */
