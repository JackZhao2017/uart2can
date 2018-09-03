
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "can_queue_send.h"
#include "uart_encode.h"
#include "task_config.h"



static   UartRxMsg  uartMsg[SEND_QUEUE_SIZE];
static   OS_EVENT *g_mutex_cansend;

void CAN_QueueSendInit(QUEUE_SEND_INFO *info,int size)
{
	uint8_t err;
	memset(info,0,sizeof(QUEUE_SEND_INFO));
	memset(uartMsg,0,sizeof(uartMsg));
	info->txbuf=uartMsg;
	info->mark=size-1;
	info->size=size;  
	g_mutex_cansend = OSMutexCreate(CANSEND_MUTEX_PRIO,&err);
	printf("%s  size of UartRxMsg %d \r\n",__func__ ,sizeof(UartRxMsg)); 
}
void CAN_QueueSendPut(QUEUE_SEND_INFO *info,uint8_t *msg)
{
	uint8_t err;
	OSMutexPend(g_mutex_cansend,0,&err);
	if(((info->put+1)&info->mark)==info->get){
		printf("put_send_queue full \r\n");
		OSMutexPost(g_mutex_cansend);
		return ;
	}
	memcpy(&info->txbuf[info->put],msg,sizeof(UartRxMsg));
	info->put=(info->put+1)&info->mark;
	OSMutexPost(g_mutex_cansend);
}

uint8_t  CAN_QueueSendGet(QUEUE_SEND_INFO *info,UartRxMsg *msg)
{
	uint8_t err;
	OSMutexPend(g_mutex_cansend,0,&err);
	if(info->get==info->put){
		OSMutexPost(g_mutex_cansend);
		return 0;
	}
	memcpy(msg,&info->txbuf[info->get],sizeof(UartRxMsg));
	info->get=(info->get+1)&info->mark;
	OSMutexPost(g_mutex_cansend);
	return 1;
}


uint8_t CAN_QueueSendIsFull(QUEUE_SEND_INFO *info)
{
	uint8_t val=0,err;
	OSMutexPend(g_mutex_cansend,0,&err);
	if(((info->put+1)&0x07)==info->get){
		val=1;
	}
	OSMutexPost(g_mutex_cansend);
	return val;
}
void CAN_QueueSendClean(QUEUE_SEND_INFO *info)
{
	info->get=0;
	info->put=0;
}




