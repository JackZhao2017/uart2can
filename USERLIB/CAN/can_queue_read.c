
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes.h"
#include "task_config.h"
#include "can_queue_read.h"
#include "uart_encode.h"


static CanRxMsg   MsgBuf[READ_QUEUE_SIZE];
static OS_EVENT *g_mutex_can;

void CAN_QueueReadInit(QUEUE_READ_INFO *info,int size)
{
	uint8_t err;
	memset(info,0,sizeof(QUEUE_READ_INFO));
	memset(MsgBuf,0,sizeof(MsgBuf));
	info->rxbuf=MsgBuf;
	info->mark=size-1;
	info->size=size;
	g_mutex_can = OSMutexCreate(CAN_MUTEX_PRIO,&err);
	printf("%s  size of CanRxMsg %d \r\n",__func__ ,sizeof(CanRxMsg));
}

void CAN_QueueReadPut(QUEUE_READ_INFO *info,CanRxMsg *msg)
{
	uint8_t err;
	OSMutexPend(g_mutex_can,0,&err);
	if(((info->put+1)&info->mark)==info->get){
		printf("full \r\n");
		OSMutexPost(g_mutex_can);
		return ;
	}
	memcpy(&info->rxbuf[info->put],msg,sizeof(CanRxMsg));
	info->put=(info->put+1)&info->mark;
	OSMutexPost(g_mutex_can);
}

uint8_t  CAN_QueueReadGet(QUEUE_READ_INFO *info,CanRxMsg *msg)
{
	uint8_t err;
	OSMutexPend(g_mutex_can,0,&err);
	if(info->get==info->put){
		OSMutexPost(g_mutex_can);
		return 0;
	}
	
	memcpy(msg,&info->rxbuf[info->get],sizeof(CanRxMsg));
	info->get=(info->get+1)&info->mark;

	OSMutexPost(g_mutex_can);
	return 1;
}
uint8_t CAN_QueueReadGetHead(QUEUE_READ_INFO *info,CanRxMsg **msg)
{
	uint8_t err;
	OSMutexPend(g_mutex_can,0,&err);
	if(info->get==info->put){
		OSMutexPost(g_mutex_can);
		return 0;		
	}
	*msg = &info->rxbuf[info->get];
	OSMutexPost(g_mutex_can);
	return 1;
}
void CAN_QueueReadDelHead(QUEUE_READ_INFO *info)
{
	uint8_t err;
	OSMutexPend(g_mutex_can,0,&err);
	info->get=(info->get+1)&info->mark;
	OSMutexPost(g_mutex_can);
}

uint8_t CAN_QueueReadIsFull(QUEUE_READ_INFO *info)
{
	uint8_t val=0,err;
	OSMutexPend(g_mutex_can,0,&err);
	if(((info->put+1)&0x07)==info->get){
		val=1;
	}
	OSMutexPost(g_mutex_can);
	return val;
}
void CAN_QueueReadClean(QUEUE_READ_INFO *info)
{
	info->get=0;
	info->put=0;
}


