/*
 * init.cpp
 *
 *  Created on: 2015/10/12
 *      Author: toyo
 */
extern "C"{
#include <osapi.h>
#include <mem.h>

#include <os_type.h>
#include <ets_sys.h>
#include <user_interface.h>

#include <espconn.h>
#include <driver/uart.h>

#include <c_types.h>
#include <ip_addr.h>

#include "espmissing.h"
}

#include "EOJNodeProfile.h"

EOJNodeProfile* node_p;

EOJNodeProfile* EOJNode(void);

#define procTaskPrio        0
#define procTaskQueueLen    1
os_event_t procTaskQueue[procTaskQueueLen];

//Tasks that happen all the time.
uint8_t printed_ip = 0;
void ICACHE_FLASH_ATTR procTask(os_event_t *events)
{
	system_os_post(procTaskPrio, 0, 0 );

	if( events->sig == 0 && events->par == 0 )
	{
		//Idle Event.
		struct station_config wcfg;
		char stret[256];
		char *stt = &stret[0];
		struct ip_info ipi;

		int stat = wifi_station_get_connect_status();

		if( stat == STATION_WRONG_PASSWORD || stat == STATION_NO_AP_FOUND || stat == STATION_CONNECT_FAIL )
		{
			wifi_station_get_config( &wcfg );
			stt += ets_sprintf( stt, "Connection failed: %d (%s,%s)\n", stat ,wcfg.ssid, wcfg.password);

			wifi_set_opmode_current( 2 );
			stt += ets_sprintf( stt, "Connection failed: %d\n", stat );
			ets_uart_printf(stret);
		}

		if( stat == STATION_GOT_IP && !printed_ip )
		{
			wifi_station_get_config( &wcfg );
			wifi_get_ip_info(0, &ipi);
			stt += ets_sprintf( stt, "STAT: %d\n", stat );
			stt += ets_sprintf( stt, "IP: %d.%d.%d.%d\n", (ipi.ip.addr>>0)&0xff,(ipi.ip.addr>>8)&0xff,(ipi.ip.addr>>16)&0xff,(ipi.ip.addr>>24)&0xff );
			stt += ets_sprintf( stt, "NM: %d.%d.%d.%d\n", (ipi.netmask.addr>>0)&0xff,(ipi.netmask.addr>>8)&0xff,(ipi.netmask.addr>>16)&0xff,(ipi.netmask.addr>>24)&0xff );
			stt += ets_sprintf( stt, "GW: %d.%d.%d.%d\n", (ipi.gw.addr>>0)&0xff,(ipi.gw.addr>>8)&0xff,(ipi.gw.addr>>16)&0xff,(ipi.gw.addr>>24)&0xff );
			stt += ets_sprintf( stt, "WCFG: /%s/%s/\n", wcfg.ssid, wcfg.password );

			ip_addr_t mgroup_ip;
			mgroup_ip.addr=0x001700e0;//224.0.23.0
			if(espconn_igmp_join(&ipi.ip,&mgroup_ip)){
				ets_uart_printf("\rError on joining multicast group. Continue\r");
			}
			stt += ets_sprintf( stt, "MC: %d.%d.%d.%d\n", (mgroup_ip.addr>>0)&0xff,(mgroup_ip.addr>>8)&0xff,(mgroup_ip.addr>>16)&0xff,(mgroup_ip.addr>>24)&0xff );

			ets_uart_printf(stret);

			node_p=EOJNode();

			node_p->annoRouting(0x0e,0xf0,0x01);
			printed_ip = 1;
		}
	}
}

void ICACHE_FLASH_ATTR udpserver_sent(void *arg){
}


//#define MAX_FRAME 2000
//Called when new packet comes in.
void ICACHE_FLASH_ATTR udpserver_recv(void *arg, char* pt, unsigned short len)
{
	struct espconn *pespconn = (struct espconn *)arg;
	//	uint8_t buffer[MAX_FRAME];
	esp_udp recvipport,sendipport;
	os_memcpy(&recvipport,pespconn->proto.udp,sizeof(recvipport));

	ets_uart_printf("\nReceiving\n");

	echoNetHeader* enh;
	echoNetData* end;
	uint8_t opc;
	uint8_t resp[1024];
	uint8_t* resp_p=resp;
	bool success=true;

	enh=(echoNetHeader*)pt;
	pt+=11;

	ets_uart_printf("EHD1=%02x, EHD2=%02x, TID=%04x, SEOJ=%02x%02x%02x, DEOJ=%02x%02x%02x, ESV=%02x, ",
			enh->ehd1,enh->ehd2,enh->tid,
			enh->sourceclassgroup,enh->sourceclass,enh->sourceinstance,
			enh->destinationclassgroup,enh->destinationclass,enh->destinationinstance,
			enh->esv);

	if(!node_p->myDEOJ(enh->destinationclassgroup,enh->destinationclass,enh->destinationinstance))
		return;
	*(resp_p++)=enh->ehd1;
	*(resp_p++)=enh->ehd2;
	*(resp_p++)=enh->tid & 0xff;
	*(resp_p++)=enh->tid >> 8;
	*(resp_p++)=enh->destinationclassgroup;
	*(resp_p++)=enh->destinationclass;
	*(resp_p++)=enh->destinationinstance;
	*(resp_p++)=enh->sourceclassgroup;
	*(resp_p++)=enh->sourceclass;
	*(resp_p++)=enh->sourceinstance;
	*(resp_p++)=enh->esv; // dummy.

	int esv=enh->esv;

	if(esv == 0x60 || esv == 0x61 ||esv == 0x6e){
		opc=(*pt++);
		*(resp_p++)=opc;
		ets_uart_printf("OPCset=%02x\n",opc);
		for(int i=1;i <= opc ; i++){
			end=(echoNetData*)pt;
			ets_uart_printf("EPC%d=%02x, PDC%d=%02x",i,end->epc,i,end->pdc);

			pt+=2;

			if(end->pdc != 0){
				ets_uart_printf(", EDT=");

				for(int j=0;j<end->pdc;j++){
					ets_uart_printf("%02x ",*pt++);
				}
			}

			ets_uart_printf("\n");


			if(node_p->setRouting(enh->destinationclassgroup,enh->destinationclass,enh->destinationinstance,
					end->epc,end->pdc,end->edt)){
				*(resp_p++)=end->epc;
				*(resp_p++)=0;
			}
			else{
				*(resp_p++)=end->epc;
				*(resp_p++)=end->pdc;
				for(int k=0;k<end->pdc;k++){
					resp_p[k]=end->edt[k];
				}
				resp_p+=end->pdc;
				success=false;
			}
		}
	}

	if(esv == 0x62 || esv == 0x6e){
		opc=(*pt++);
		*(resp_p++)=opc;
		ets_uart_printf("OPCget=%02x\n",opc);
		for(int i=1;i <= opc ; i++){
			end=(echoNetData*)pt;
			ets_uart_printf("EPC%d=%02x, PDC%d=%02x",i,end->epc,i,end->pdc);

			pt+=2;

			if(end->pdc != 0){
				ets_uart_printf(", EDT=");

				for(int j=0;j<end->pdc;j++){
					ets_uart_printf("%02x ",*pt++);
				}
			}

			ets_uart_printf("\n");
			Response r;
			if(node_p->getRouting(r,enh->destinationclassgroup,enh->destinationclass,enh->destinationinstance,
					end->epc,end->pdc,end->edt)){
				*(resp_p++)=end->epc;
				*(resp_p++)=r.getPdc();
				for(int k=0;k<r.getPdc();k++){
					*(resp_p++)=r.getEdt()[k];
				}
			}
			else{
				*(resp_p++)=end->epc;
				*(resp_p++)=end->pdc;
				for(int k=0;k<end->pdc;k++){
					resp_p[k]=end->edt[k];
				}
				resp_p+=end->pdc;
				success=false;
			}
		}
	}

	if(success){
		switch(esv){
		case 0x60:
			esv=0x00;
			break;
		case 0x61:
			esv=0x71;
			break;
		case 0x62:
			esv=0x72;
			break;
		case 0x6e:
			esv=0x7e;
			break;
		case 0x63:
			esv=0x73;
			break;
		case 0x74:
			esv=0x7a;
			break;
		default:
			return;
		}
	}
	else{
		switch(esv){
		case 0x60:
			esv=0x50;
			break;
		case 0x61:
			esv=0x51;
			break;
		case 0x62:
			esv=0x52;
			break;
		case 0x6e:
			esv=0x5e;
			break;
		case 0x63:
			esv=0x53;
			break;
		case 0x74:
			esv=0x00;
			break;
		default:
			return;
		}
	}
	resp[10]=esv;
	if(esv){
		ets_uart_printf("Send: EHD1=%02x, EHD2=%02x, TID=%04x, SEOJ=%02x%02x%02x, DEOJ=%02x%02x%02x, ESV=%02x,...\n",
				enh->ehd1,enh->ehd2,enh->tid,
				enh->destinationclassgroup,enh->destinationclass,enh->destinationinstance,
				enh->sourceclassgroup,enh->sourceclass,enh->sourceinstance,
				esv);

		for(uint8_t* k=resp;k<resp_p;k++){
			ets_uart_printf("%02x ",*k);
		}

		pespconn->proto.udp->remote_port=3610;

		if(!espconn_send(pespconn,resp,resp_p-resp)){
			ets_uart_printf("\n%d.%d.%d.%d:%d %d.%d.%d.%d:%d\n",
					pespconn->proto.udp->local_ip[0],
					pespconn->proto.udp->local_ip[1],
					pespconn->proto.udp->local_ip[2],
					pespconn->proto.udp->local_ip[3],

					pespconn->proto.udp->local_port,

					pespconn->proto.udp->remote_ip[0],
					pespconn->proto.udp->remote_ip[1],
					pespconn->proto.udp->remote_ip[2],
					pespconn->proto.udp->remote_ip[3],

					pespconn->proto.udp->remote_port);
			ets_uart_printf("SENT!\n"); // prints !!!Hello World!!!
		}
		else{
			ets_uart_printf("SENT err!\n"); // prints !!!Hello World!!!
		}
	}
	else{
		ets_uart_printf("No Resp\n"); // prints !!!Hello World!!!
	}
}


struct espconn *pUdpServer;

extern "C" ICACHE_FLASH_ATTR void user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	int wifiMode = wifi_get_opmode();

	os_printf("\nCustom Server\n");

	struct station_config stationConf;
	wifi_set_opmode( 1 ); //We broadcast our ESSID, wait for peopel to join.
	os_memcpy(&stationConf.ssid, SSID , ets_strlen( SSID ) + 1);
	os_memcpy(&stationConf.password, PASSWORD , ets_strlen( PASSWORD ) + 1);
	wifi_set_opmode( 1 );
	wifi_station_set_config(&stationConf);
	wifi_station_connect();

	pUdpServer = (struct espconn *)pvPortZalloc(sizeof(struct espconn));
	ets_memset( pUdpServer, 0, sizeof( struct espconn ) );
	espconn_create( pUdpServer );
	pUdpServer->type = ESPCONN_UDP;
	pUdpServer->state = ESPCONN_NONE;
	pUdpServer->proto.udp = (esp_udp *)pvPortZalloc(sizeof(esp_udp));
	pUdpServer->proto.udp->local_port = 3610;

	espconn_regist_recvcb(pUdpServer, udpserver_recv);
	espconn_regist_sentcb(pUdpServer, udpserver_sent);

	wifi_station_dhcpc_start();

	if( espconn_create( pUdpServer ) )
	{
		while(1) { ets_uart_printf( "\rFAULT\r" ); }
	}

	//Add a process
	system_os_task(procTask, procTaskPrio, procTaskQueue, procTaskQueueLen);
	ets_uart_printf("\rBooting\r");
	system_os_post(procTaskPrio, 0, 0 );



}




// for new/delete operator
void *operator new(size_t s) {
	return pvPortMalloc(s);
}

void *operator new[](size_t s) {
	return pvPortMalloc(s);
}

void operator delete(void * p) {
	vPortFree(p);
}

void operator delete[](void * p) {
	if(p)
		vPortFree(p);
}

// for pure virtual function
extern "C" void __cxa_pure_virtual(void) __attribute__ ((__noreturn__));
extern "C" void __cxa_deleted_virtual(void) __attribute__ ((__noreturn__));
extern "C" void ICACHE_FLASH_ATTR abort() {
	ets_uart_printf("abort()");
	system_restart();
	while(1);
}

void ICACHE_FLASH_ATTR __cxa_pure_virtual(void) {
	abort();
}

void ICACHE_FLASH_ATTR __cxa_deleted_virtual(void) {
	abort();
}

// for staic variables;
typedef int __guard __attribute__((mode (__DI__)));
extern "C" int ICACHE_FLASH_ATTR __cxa_guard_acquire(__guard *g)
{
	return !*(char *)(g);
}
extern "C" void ICACHE_FLASH_ATTR __cxa_guard_release (__guard *g)
{
	*(char *)g = 1;
}
extern "C" void ICACHE_FLASH_ATTR __cxa_guard_abort (__guard *)
{
}

extern "C" int ICACHE_FLASH_ATTR atexit(void (*func)(void))
{
	return 0;
}
