/**
 * @file Response.cpp
 * @brief Response data implementation.
 * @author toyo
 * @date 2015/10/02
 */

#include "Response.h"

void ICACHE_FLASH_ATTR Response::set(uint8_t pdc_arg,const uint8_t* edt_arg){
		pdc=pdc_arg;
		for(int i=0;i<pdc;i++){
			edt[i]=edt_arg[i];
		}
	}
