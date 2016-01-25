# EchoNET8266
EchoNET Lite client for ESP8266_SDK

This is an EchoNET client for ESP8266_SDK on ESP-WROOM-02 and BMP180 (and compatible) connected by i2c.

EchoNET Lite specification is http://echonet.jp/spec_v111_lite_en/ .

This program use https://github.com/CHERTS/esp8266-devkit/ to build.  
Also, You need to copy EchoNET/include/driver/* and EchoNET/driver/* from https://github.com/CHERTS/esp8266-i2c_bmp180/ .

First, you need to check main.c .

NodeProfile of EchoNET is implemented in EOJNodeProfile.  
This class should be hold all of the class of this node.

In this sample, MyLED is implementation of GeneralLightingProfile (0x02 0x90).  
MyLED dev(4,1) switch GPIO 4 LED by GeneralLightingProfile instance code 1 (0x02 0x90 0x01)  
MyLED d2(5,2) switch GPIO 5 LED by GeneralLightingProfile instance code 2 (0x02 0x90 0x02)

Second, you need to change SSID and PASSWORD in config.h.  

Thanks.
