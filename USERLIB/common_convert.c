#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "can.h"
#include "crc8.h"
#include "uart_encode.h"
#include "uart_detect.h"


void  canconvertuart(CanRxMsg *canmsg,UartTxMsg *uartmsg)
{
	if(canmsg->IDE==CAN_ID_EXT){
		uartmsg->IDE_RTR=(1<<4)|(canmsg->RTR&0x0f);
		uartmsg->ID=canmsg->ExtId;
	}else if(canmsg->IDE==CAN_ID_STD){
		uartmsg->IDE_RTR=(0<<4)|(canmsg->RTR&0x0f);
		uartmsg->ID=canmsg->StdId;				 
	}
	uartmsg->DLC=canmsg->DLC;
	memcpy(uartmsg->Data,canmsg->Data,8);
	uartmsg->SYNC=MSY_SYNC;
	uartmsg->TYPE=CANMSG;
	uartmsg->CRC8=crc8_creator((uint8_t*)(&uartmsg->TYPE),0,CANMSG-1);
}

void uartconvertcan(UartRxMsg *uartmsg,CanTxMsg *canmsg)
{
	if((uartmsg->IDE_RTR&0xf0)==0x10){
		canmsg->IDE=1;
		canmsg->ExtId=uartmsg->ID;
	}else{
		canmsg->IDE=0;
		canmsg->StdId=uartmsg->ID;				 
	}

	if((uartmsg->IDE_RTR&0x0f)==2){
		canmsg->RTR=2;
	}else{
		canmsg->RTR=0;
	}

	canmsg->DLC=uartmsg->DLC;
	memcpy(canmsg->Data,uartmsg->Data,8);
}


