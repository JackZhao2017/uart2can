
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "can_queue.h"
#include "Encode.h"

static CanRxMsg MsgBuf[CAN_QUEUE_SIZE];
static UartRxMsg  uartMsg[SEND_QUEUE_SIZE];

void can_queue_init(QUEUE_INFO *info,uint8_t size)
{
	memset(info,0,sizeof(QUEUE_INFO));
	memset(MsgBuf,0,sizeof(MsgBuf));
	info->rxbuf=MsgBuf;
	info->mark=size-1;
	info->size=size;
	printf("%s  size of CanRxMsg %d \r\n",__func__ ,sizeof(CanRxMsg));
}

void put_data_queue(QUEUE_INFO *info,CanRxMsg *msg)
{
	if(((info->put+1)&info->mark)==info->get){
			printf("full \r\n");
			return ;
	}
	memcpy(&info->rxbuf[info->put],msg,sizeof(CanRxMsg));
	info->put=(info->put+1)&info->mark;
}

uint8_t  get_data_queue(QUEUE_INFO *info,CanRxMsg *msg)
{
	if(info->get==info->put)
		return 0;
	memcpy(msg,&info->rxbuf[info->get],sizeof(CanRxMsg));
	info->get=(info->get+1)&info->mark;
	return 1;
}

uint8_t is_queue_Full(QUEUE_INFO *info)
{
	 return (((info->put+1)&0x07)==info->get);
}





void can_sendqueue_init(QUEUE_INFO *info,uint8_t size)
{
		memset(info,0,sizeof(QUEUE_INFO));
		memset(uartMsg,0,sizeof(uartMsg));
		info->txbuf=uartMsg;
		info->mark=size-1;
		info->size=size;  
		printf("%s  size of UartRxMsg %d \r\n",__func__ ,sizeof(UartRxMsg)); 
}
void put_send_queue(QUEUE_INFO *info,uint8_t *msg)
{
		if(((info->put+1)&info->mark)==info->get){
				printf("put_send_queue full \r\n");
				return ;
		}
		memcpy(&info->txbuf[info->put],msg,sizeof(UartRxMsg));
		info->put=(info->put+1)&info->mark;
}


uint8_t  get_send_queue(QUEUE_INFO *info,UartRxMsg *msg)
{
	if(info->get==info->put)
		return 0;
	memcpy(msg,&info->txbuf[info->get],sizeof(UartRxMsg));
	info->get=(info->get+1)&info->mark;
	return 1;
}


uint8_t is_sendqueue_Full(QUEUE_INFO *info)
{
	 return (((info->put+1)&0x07)==info->get);
}





