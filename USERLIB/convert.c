#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "can.h"
#include "Encode.h"
#include "crc8.h"

void  canconvertuart(CanRxMsg *canmsg,UartTxMsg *uartmsg)
{
			if(canmsg->IDE==CAN_ID_EXT){
				 uartmsg->IDE=1;
				 uartmsg->ID=canmsg->ExtId;
			}else if(canmsg->IDE==CAN_ID_STD){
				 uartmsg->IDE=0;
				 uartmsg->ID=canmsg->StdId;				 
			}
			uartmsg->DLC=canmsg->DLC;
			uartmsg->RTR=canmsg->RTR;
			memcpy(uartmsg->Data,canmsg->Data,8);
			uartmsg->SYNC=0x55;
			uartmsg->TYPE=0x10;
			uartmsg->CRC8=crc8_creator((uint8_t*)uartmsg,1,15);
}

void uartconvertcan(UartRxMsg *uartmsg,CanTxMsg *canmsg)
{
	   	if(uartmsg->IDE==1){
				 canmsg->IDE=1;
				 canmsg->ExtId=uartmsg->ID;
		}else if(uartmsg->IDE==0){
				 canmsg->IDE=0;
				 canmsg->StdId=uartmsg->ID;				 
		}
		canmsg->DLC=uartmsg->DLC;
		memcpy(canmsg->Data,uartmsg->Data,8);
}


