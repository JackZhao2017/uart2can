/*
	Created by Jack 2017/9/7
*/
#include "can.h"
#include "includes.h"
#include "delay.h"
#include "taskconfig.h"
#include "can_speed.h"
#include "stdio.h"

OS_STK 	 OBDII_TASK_STK[CAN_OBDII_STK_SIZE];
static 	 u8 resolver_buf[8];

static 	 OS_EVENT * sem_obd;
static 	 u8 g_taskstatus=0;

volatile u8 g_isfinished=1;

s8 set_obdii_task_status(u8 status)
{
	 if(status==TASK_ON){
			if(!g_taskstatus){
					g_taskstatus=TASK_ON;
					OSTaskResume(CAN_OBDII_PRIO);
			}
	 }else if(status==TASK_OFF){
			if(g_taskstatus){
					g_taskstatus=TASK_OFF;
					OSTaskSuspend(CAN_OBDII_PRIO);
			}	 
	 }
	 return g_taskstatus;
}
u8 get_obdii_task_status(void)
{
		return g_taskstatus;
}
void resolve_obdii_speed(u8 *data)
{
		if(!g_isfinished)
			return ;
		memcpy(resolver_buf,data,8);
		OSSemPost(sem_obd);
}

void obdii_task(void *pdata)
{	
		s8 ret=0;
		u8 err;
		u8 timeoutcount=0;
		sem_obd=OSSemCreate(0);
		g_taskstatus=0;
		g_isfinished=1;
		//OSTaskSuspend(CAN_OBDII_PRIO);
		printf("obdii_task init \r\n");
		while(1)
		{
				ret = obdii_requeset(OBDII_SPEED_PID);
				if(!ret){
						OSSemPend(sem_obd,100,&err);
					  switch(err)
						{
								case OS_ERR_NONE:
										timeoutcount=0;
										g_isfinished=0;
										can_obdii_resolve_speed(resolver_buf);
										g_isfinished=1;
										break;
								case OS_ERR_PEND_ABORT:
										break;
								case OS_ERR_TIMEOUT:
										if(timeoutcount>3){
												set_system_status(0);
										}else{
												++timeoutcount;
										}
										break;
								default:
										break;
						}
				}else{
						printf("obdii_requeset faild\r\n");
				}					
		}
}

