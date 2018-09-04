/*	Created by Jack 2017/9/6 */


#include "task_config.h"
#include "can_speed.h"
#include "led.h"
#include "delay.h"
#include "includes.h"
#include "can.h"
#include "usart2.h"
#include "task_schedule.h"
#include "task_can.h"
#include "sleepwakeup.h"
#include "keyAcc.h"
//LEDÈÎÎñ



OS_STK    SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE];
volatile	s8 g_system_state=0;

#define  PERIOD  10
static	u16  ledcount=0;
static	u8   led_on=1;
static	u8	 scale=2;
static	u16  period=PERIOD;

void displaystatus(void)
{
	if(g_system_state>0){
	  period=PERIOD;
		scale =period/2;	
	}else if(g_system_state<0){
		period=PERIOD/2;
		scale =period/2;
	}else if(g_system_state==0){
		period=PERIOD;
		scale=period/5;
	}			
	if(ledcount<scale){
		led_on=1;
	}else if(ledcount>=period){
		ledcount=0;
	}else{
		led_on=0;
	}
	if(led_on){					
		LED0=0;
		LED_ON();					
	}else{
		LED0=1;
		LED_OFF();
	}
	++ledcount;
}

void SCHEDULE_SetSystemStatus(s8 status)
{
		g_system_state=status;
}
void schedule_indicate_task(void *pdata)
{	 
	u8 AccOrg=KEYACC_Scan();
	u32 count=0;
	while(1)
	{							
		displaystatus();;
		if((g_system_state<=0)&&KEYACC_Scan()){
			if(++count==30){
				count=0;
				entry_sleep_mode();
			}
		}else{
			count=0;
		}
		OSTimeDlyHMSM(0,0,1,0);			
	}
}
void SCHEDULE_TaskInit(void)
{
	INT16U id=SCHEDULE_INDICATE_TASK_PRIO;
	OSTaskCreateExt(schedule_indicate_task,
					(void *)0,
					(OS_STK *)&SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE-1],
					 SCHEDULE_INDICATE_TASK_PRIO,
					 id,
					(OS_STK *)&SCHEDULE_INDICATE_TASK_STK[0],
					 SCHEDULE_INDICATE_STK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK
							  ); 
	//OSTaskCreate(schedule_indicate_task,(void *)0,(OS_STK*)&SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE-1],SCHEDULE_INDICATE_TASK_PRIO);
}


