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

OS_STK 	 CAN_SAE_TASK_STK[CAN_SAE_STK_SIZE];


static 	 CanRxMsg g_RxMsg;
static 	 OS_EVENT * g_sem_can;
volatile u8 g_isfinished=1;


void resolve_can_message(CanRxMsg *msg)
{
		if(!g_isfinished)
			return ;
		memcpy(&g_RxMsg,msg,sizeof(CanRxMsg));
		OSSemPost(g_sem_can);
}
void can_sae_task(void *pdata)
{
		u8 err;
		g_isfinished=1;
		printf("can_sae_task init \r\n");
		while(1)
		{		
				OSSemPend(g_sem_can,2000,&err);
				switch(err)
				{
								case OS_ERR_NONE:
										g_isfinished=0;	
										//printf("receive ext id =0x%x\r\n",g_RxMsg.ExtId);								
										set_system_status(SAE_1939);
									  can_resolve_speed(g_RxMsg.Data,7);	
										set_canbus_curmode(SAE_1939);
										set_canbus_datamode(VAILD);							
										g_isfinished=1;
										break;
								case OS_ERR_PEND_ABORT:
										break;
								case OS_ERR_TIMEOUT:
										set_canbus_datamode(INVAILD);
										set_system_status(0);
										break;
								default:
										break;
				}				
		}
}


void can_task_init(void)
{
		g_sem_can=OSSemCreate(0);
		OSTaskCreate(can_sae_task,(void *)0,(OS_STK*)&CAN_SAE_TASK_STK[CAN_SAE_STK_SIZE-1],CAN_SAE_PRIO);	
}
