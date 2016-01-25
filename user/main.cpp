/*
 * main.cpp
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */

#include "EOJNodeProfile.h"
#include "EOJGeneralLighting.h"
#include "EOJTemperatureSensor.h"
#include "EOJAirPressureSensor.h"
#include "BMP180.h"
extern "C"{
#include <ets_sys.h>
#include <gpio.h>
}


class MyLED: public EOJGeneralLighting{
int gpio;
	void becomesOn(){
		GPIO_OUTPUT_SET(GPIO_ID_PIN(gpio), 1);
	}
	void becomesOff(){
		GPIO_OUTPUT_SET(GPIO_ID_PIN(gpio), 0);
	}
public:
	MyLED(int g,int inst) : gpio(g),EOJGeneralLighting(inst){
	}
};

EOJNodeProfile* ICACHE_FLASH_ATTR EOJNode(void){
	static MyLED dev(4,1);
	static MyLED d2(5,2);
	static BMP180 s;
	static EOJNodeProfile np(&dev);

	np.addDevice(&d2);

	static EOJTemperatureSensor temp(&s,1);
	static EOJAirPressureSensor pres(&s,1);


	np.addDevice(&temp);
	np.addDevice(&pres);

	return &np;
}
