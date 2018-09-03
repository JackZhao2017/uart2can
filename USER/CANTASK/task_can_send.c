/*
	Created by Jack 2017/9/7
*/
#include "includes.h"
#include "stdio.h"
#include "delay.h"
#include "task_config.h"
#include "common_convert.h"
#include "uart_encode.h"
#include "can_queue_send.h"

OS_STK 	 CAN_SEND_TASK_STK[CAN_SEND_STK_SIZE];
static 	 OS_EVENT *g_sem_cansend;
static   volatile u8  g_cansend_status=0;
volatile u8 g_istxfinished=1;
QUEUE_SEND_INFO CAN_SEND_INFO;



static  uint8_t CAN_GetSendQueue(UartRxMsg * msg)
{
	if(CAN_QueueSendGet(&CAN_SEND_INFO,msg))
	{
		return 1;	
	}
	return 0;
}
void CAN_SendOneFrame(u8 *msg)
{
	CAN_QueueSendPut(&CAN_SEND_INFO,msg);
	if(!g_istxfinished){
		printf("can send task busy!\r\n");
		return ;
	}
	OSSemPost(g_sem_cansend);
}


static void CanSendTask(void *pdata)
{
	INT8U      err=0;
	UartRxMsg  uartMsg;
	CanTxMsg   canmsg;
	g_istxfinished=1;
	g_cansend_status=1;
	printf("cansendTask init \r\n");
	while(1)
	{
		g_istxfinished=0;		
		if(CAN_GetSendQueue(&uartMsg))
		{
			uartconvertcan(&uartMsg,&canmsg);
			CAN_TRANSMIT(&canmsg);
			OSTimeDlyHMSM(0,0,0,10);
		}else{
			g_istxfinished=1;
			OSSemPend(g_sem_cansend,0,&err);
		}
	}
}

void CAN_SendTaskInit(void)
{
	INT16U id_send=CAN_SEND_PRIO;
	CAN_QueueSendInit(&CAN_SEND_INFO,SEND_QUEUE_SIZE);
	g_sem_cansend   = OSSemCreate(0);		
	OSTaskCreateExt(CanSendTask,
						(void *)0,
						(OS_STK *)&CAN_SEND_TASK_STK[CAN_SEND_STK_SIZE-1],
						CAN_SEND_PRIO,
						id_send,
						(OS_STK *)&CAN_SEND_TASK_STK[0],
						CAN_SEND_STK_SIZE,
						(void *)0,
						OS_TASK_OPT_STK_CHK
					); 
	printf("%s\r\n",__func__);
}

uint8_t CAN_SendTaskPause(void)
{
	INT8U err=0,count=0;
	if(!g_cansend_status){
		return 1;
	}	 
	err = OSTaskSuspend(CAN_SEND_PRIO);
	while(err){
		printf("pause_send_can_task err =%d \r\n",err);
		err = OSTaskSuspend(CAN_SEND_PRIO);
		if(++count==5){
			return 0;
		}
	}
	g_cansend_status=0;

	return 1;
}	

uint8_t CAN_SendTaskStart(void)
{
	INT8U err=0,count=0;
	if(g_cansend_status){
		return 1;
	}
	err = OSTaskResume(CAN_SEND_PRIO);
	while(err){
		printf("pause_read_can_task err =%d \r\n",err);
		err = OSTaskResume(CAN_SEND_PRIO);
		if(++count==5){
			return 0;
		}
	}
	CAN_QueueSendClean(&CAN_SEND_INFO);
	g_cansend_status=1;
	printf("start_cansend_task resume\r\n");
	return 1;
}

uint8_t CAN_GetSendTaskStatus(void)
{
	return g_cansend_status;
}


