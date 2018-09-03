
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart_encode.h"
#include "can_speed.h"
#include "task_can.h"
#include "common_convert.h"
#include "crc8.h"
extern CanRxMsg  g_RxMsg;
static uint16_t msgcounter=0;
static uint8_t  sys_status=0;
static uint8_t  MsgCounter(void)
{
		uint8_t retval=msgcounter&0xff;
	    msgcounter=(msgcounter+1)&0xff;
		return retval;
}

uint8_t UART_ENCODE_GetSend(UartTxMsg *uartmsg)
{
	CanRxMsg *canmsg;
	if(CAN_GetHeadReadMsg(&canmsg)){
		uartmsg->COUNTER=MsgCounter();
		canconvertuart(canmsg,uartmsg);
		CAN_DelHeadReadMsg();
		return 1;
	}
	return 0;
}

void UART_ENCODE_AddSystermErrorcode(uint8_t err)
{
	sys_status=err;
}

void UART_ENCODE_GetSystermStatus(SysMsg *sysmsg)
{
	sysmsg->SYNC		=0x55;
	sysmsg->TYPE		=SYSMSG; 
	sysmsg->DATA_S  =SPEED_GetCanBusStatus();
	sysmsg->TASK_S  =0;
	if(CAN_GetSendTaskStatus()){
		sysmsg->TASK_S |=0x1;
	}
	if(CAN_GetReadTaskStatus()){
		sysmsg->TASK_S |=0x2;
	}	
	sysmsg->BITRATE=CAN_GetBrandrate();
	sysmsg->ERRCODE=sys_status;	
	sysmsg->COUNTER=MsgCounter();
	sysmsg->CRC8   =crc8_creator((uint8_t*)(&sysmsg->TYPE),0,SYSMSG-1);
}


