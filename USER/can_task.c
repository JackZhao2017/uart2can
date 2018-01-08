/*
	Created by Jack 2017/9/7
*/
#include "can.h"
#include "includes.h"
#include "delay.h"
#include "taskconfig.h"
#include "can_speed.h"
#include "stdio.h"
#include "can_task.h"
#include "schedule_task.h"
#include "can_queue.h"
#include "uart_task.h"
#include "Encode.h"
#include "usart2.h"
#include "convert.h"

OS_STK 	 CAN_READ_TASK_STK[CAN_READ_STK_SIZE];
OS_STK 	 CAN_SEND_TASK_STK[CAN_SEND_STK_SIZE];

FILTER filter[FILTER_NUM]={{0x0cfe6c00,0x00FFFF00,EXT}};

static 	 CanRxMsg  g_RxMsg;

static 	 OS_EVENT *g_sem_can;
static   OS_EVENT *g_mutex_can;

static   u8  g_canread_status=0;
static   u8  g_cansend_status=0;

static 	 OS_EVENT *g_sem_cansend;
static   OS_EVENT *g_mutex_cansend;

volatile u8 g_istxfinished=1;
volatile u8 g_isfinished=1;



QUEUE_INFO CAN_QUEUE_INFO;
QUEUE_INFO CAN_SEND_INFO;

uint8_t getCanMsg(CanRxMsg *msg)
{
	    uint8_t err;
		OSMutexPend(g_mutex_can,0,&err);
		if(get_data_queue(&CAN_QUEUE_INFO,msg)){
			OSMutexPost(g_mutex_can);
			return 1;
		}
		OSMutexPost(g_mutex_can);
		return 0;
}


void resolve_can_message(CanRxMsg *msg)
{
		if(!g_isfinished)
			return ;
		g_isfinished=0;
		memcpy(&g_RxMsg,msg,sizeof(CanRxMsg));
		OSSemPost(g_sem_can);
}

void canreadTask(void *pdata)
{
		u8 		  err;
		g_isfinished=1;
		g_canread_status=1;
		printf("canreadTask init \r\n");
		while(1)
		{		
				OSSemPend(g_sem_can,2000,&err);			
				switch(err)
				{
					case OS_ERR_NONE:	
							OSMutexPend(g_mutex_can,0,&err);
							put_data_queue(&CAN_QUEUE_INFO,&g_RxMsg);
							OSMutexPost(g_mutex_can);
							uartsendstart();
							g_isfinished=1;
							set_system_status(1);
							set_canbus_status(VAILD);														
							break;
					case OS_ERR_PEND_ABORT:
							break;
					case OS_ERR_TIMEOUT:
							set_canbus_status(INVAILD);
							set_system_status(0);
							break;
					default:
							break;
				}			
		}
}




static  uint8_t _getcansendPacket(UartRxMsg * msg)
{
	uint8_t err;
	OSMutexPend(g_mutex_cansend,0,&err);
	if(get_send_queue(&CAN_SEND_INFO,msg))
	{
		OSMutexPost(g_mutex_cansend);
		return 1;	
	}
	OSMutexPost(g_mutex_cansend);
	return 0;
}
void putcansendPacket(u8 *msg)
{
	 uint8_t err;
	 if(!g_cansend_status){
	 	//printf("%s  cansendtask suspend\r\n",__func__);
	 	return ;
	 }
	 OSMutexPend(g_mutex_cansend,0,&err);
	 put_send_queue(&CAN_SEND_INFO,msg);
	 OSMutexPost(g_mutex_cansend);
}
void startcansend(void)
{
		if((!g_istxfinished)&&g_cansend_status){
			return ;
		}
		OSSemPost(g_sem_cansend);
}

void cansendTask(void *pdata)
{
		INT8U      err=0;
		UartRxMsg  uartMsg;
		CanTxMsg   canmsg;
	    g_istxfinished=1;
	    g_cansend_status=1;
	    printf("cansendTask init \r\n");
		while(1)
		{
			OSSemPend(g_sem_cansend,0,&err);
			g_istxfinished=0;		
			if(_getcansendPacket(&uartMsg))
			{
				 uartconvertcan(&uartMsg,&canmsg);
				 CAN_TRANSMIT(&canmsg);
			}
			g_istxfinished=1;
		}
}

void can_task_init(BAUDRATE brandrate)
{
		INT8U  err;
		can_queue_init(&CAN_QUEUE_INFO,CAN_QUEUE_SIZE);
		can_sendqueue_init(&CAN_SEND_INFO,SEND_QUEUE_SIZE);
		CAN_INIT(brandrate);
		CAN_FILTERINIT(filter,1);	
	
		g_sem_can=OSSemCreate(0);
		g_mutex_can =OSMutexCreate(CAN_MUTEX_PRIO,&err);
		OSTaskCreate(canreadTask,(void *)0,(OS_STK*)&CAN_READ_TASK_STK[CAN_READ_STK_SIZE-1],CAN_READ_PRIO);

		g_sem_cansend   =OSSemCreate(0);
		g_mutex_cansend =OSMutexCreate(CANSEND_MUTEX_PRIO,&err);
		OSTaskCreate(cansendTask,(void *)0,(OS_STK*)&CAN_SEND_TASK_STK[CAN_SEND_STK_SIZE-1],CAN_SEND_PRIO);	
	    CAN_NVIC_Configuration();   
	    printf("can_task_init\r\n");
}

void pause_send_can_task(void)
{
	 INT8U err=0,count=0;
	 if(!g_cansend_status){
	 	 return ;
	 }	 
	 err = OSTaskSuspend(CAN_SEND_PRIO);
	 while(err){
	 	printf("pause_send_can_task err =%d \r\n",err);
	 	err = OSTaskSuspend(CAN_SEND_PRIO);
	 	if(++count==5){
	 		return ;
	 	}
	 }
	 g_cansend_status=0;
	 printf("pause_send_can_task paused \r\n");
}	

void pause_read_can_task(void)
{
	 INT8U err=0,count=0;
	 if(!g_canread_status){
	 	 return ;
	 }
	 err = OSTaskSuspend(CAN_READ_PRIO);
	 while(err){
	 	printf("pause_read_can_task err =%d \r\n",err);
	 	err = OSTaskSuspend(CAN_READ_PRIO);
	 	if(++count==5){
	 		return ;
	 	}
	 }
	 g_canread_status=0;
	 printf("pause_read_can_task paused \r\n");
}

void pause_can_task(void)
{
	 pause_send_can_task();
	 pause_read_can_task();
}

void start_canread_task(void)
{
	 INT8U err=0,count=0;
	 if(g_canread_status){
	 	 return ;
	 }
	 err = OSTaskResume(CAN_READ_PRIO);
	 while(err){
	 	printf("pause_read_can_task err =%d \r\n",err);
	 	err = OSTaskResume(CAN_READ_PRIO);
	 	if(++count==5){
	 		return ;
	 	}
	 }
	 g_canread_status=1;
	 printf("start_canread_task resume\r\n");

}
void start_cansend_task(void)
{
	 INT8U err=0,count=0;
	 if(g_cansend_status){
	 	 return ;
	 }
	 err = OSTaskResume(CAN_SEND_PRIO);
	 while(err){
	 	printf("pause_read_can_task err =%d \r\n",err);
	 	err = OSTaskResume(CAN_SEND_PRIO);
	 	if(++count==5){
	 		return ;
	 	}
	 }
	 g_cansend_status=1;
	 printf("start_cansend_task resume\r\n");
}


void can_set_brandrate(BAUDRATE brandrate)
{
	 CAN_INIT(brandrate);
}

void can_set_filter(FILTER *filter,uint8_t len)
{
	 CAN_FILTERINIT(filter,len);	
}

