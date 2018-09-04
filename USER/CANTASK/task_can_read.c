/*
	Created by Jack 2017/9/7
*/

#include "includes.h"
#include "delay.h"
#include "task_config.h"
#include "can_speed.h"
#include "stdio.h"
#include "task_schedule.h"
#include "can_queue_read.h"



OS_STK 	 CAN_READ_TASK_STK[CAN_READ_STK_SIZE];

static 	 OS_EVENT *g_sem_can;
static   volatile u8  g_canread_status=0;
volatile u8   g_isfinished=1;

CanRxMsg  g_RxMsg;
QUEUE_READ_INFO CAN_QUEUE_INFO;

void CAN_ResolveMsg(CanRxMsg *msg)
{
	if(!g_isfinished)
		return ;
	g_isfinished=0;
	memcpy(&g_RxMsg,msg,sizeof(CanRxMsg));
	OSSemPost(g_sem_can);
}

static void canreadTask(void *pdata)
{
	u8 err=0;
	g_isfinished=1;
	g_canread_status=1;
	printf("canreadTask init \r\n");
	while(1)
	{		
		OSSemPend(g_sem_can,500,&err);			
		switch(err)
		{
			case OS_ERR_NONE:
				SPEED_SetCanBusStatus(VAILD);					
				CAN_QueueReadPut(&CAN_QUEUE_INFO,&g_RxMsg);
				g_isfinished=1;
				SCHEDULE_SetSystemStatus(1);																				
				break;
			case OS_ERR_PEND_ABORT:
				break;
			case OS_ERR_TIMEOUT:
				SPEED_SetCanBusStatus(INVAILD);
				SCHEDULE_SetSystemStatus(0);
				break;
			default:
				break;
		}			
	}
}






void CAN_ReadTaskInit(void)
{
	INT16U id_read=CAN_READ_PRIO;
	CAN_QueueReadInit(&CAN_QUEUE_INFO,READ_QUEUE_SIZE);
	g_sem_can   = OSSemCreate(0);		
	OSTaskCreateExt(canreadTask,
					(void *)0,
					(OS_STK *)&CAN_READ_TASK_STK[CAN_READ_STK_SIZE-1],
					CAN_READ_PRIO,
					id_read,
					(OS_STK *)&CAN_READ_TASK_STK[0],
					CAN_READ_STK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK
	); 
	printf("%s\r\n",__func__);
}
uint8_t CAN_ReadTaskPause(void)
{
	INT8U err=0,count=0;
	if(!g_canread_status){
		return 1;
	}
	err = OSTaskSuspend(CAN_READ_PRIO);
	while(err){
		printf("pause_read_can_task err =%d \r\n",err);
		err = OSTaskSuspend(CAN_READ_PRIO);
		if(++count==5){
			return 0;
		}
	}
	g_canread_status=0;

	return 1;
}


uint8_t CAN_ReadTaskStart(void)
{
	INT8U err=0,count=0;
	if(g_canread_status){
		return 1;
	}
	err = OSTaskResume(CAN_READ_PRIO);

	while(err){

		printf("pause_read_can_task err =%d \r\n",err);
		err = OSTaskResume(CAN_READ_PRIO);
		if(++count==5){
			return 0;
		}
	}	 
	CAN_QueueReadClean(&CAN_QUEUE_INFO);
	g_canread_status=1;
	printf("start_canread_task resume\r\n");
	return 1;

}

uint8_t CAN_GetReadTaskStatus(void)
{
	return g_canread_status;
}


