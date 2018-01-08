/*	Created by Jack 2017/9/6 */


#include "taskconfig.h"
#include "can_speed.h"
#include "led.h"
#include "delay.h"
#include "includes.h"
#include "can.h"
#include "schedule_task.h"
#include "can_task.h"
#include "usart2.h"
#include "taskconfig.h"
#include "cmd_queue.h"
#include "control.h"

//LEDÈÎÎñ



OS_STK    SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE];
OS_TMR    *tmr;
u8        tmrname[]="tmr";
volatile	s8 g_system_state=0;

void set_system_status(s8 status)
{
		g_system_state=status;
}
static	u16  ledcount=0;
static	u8   led_on=1;
static	u8	 scale=20;
static	u16  period=100;

void displaystatus(void)
{
	if(g_system_state>0){
		scale=50;
		period=100;
	}else if(g_system_state<0){
		period=50;
		scale =period/2;
	}else if(g_system_state==0){
		scale=20;
		period=100;
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
		LED1=1;
		LED_ON();					
	}else{
		LED0=1;
		LED1=0;
		LED_OFF();
	}
	++ledcount;
}

void schedule_indicate_task(void *pdata)
{	 	

		while(1)
		{							
				displaystatus();
				control();
				OSTimeDlyHMSM(0,0,0,10);			
		}
}
void schedule_task_init(void)
{
			controlInit();
			OSTaskCreate(schedule_indicate_task,(void *)0,(OS_STK*)&SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE-1],SCHEDULE_INDICATE_TASK_PRIO);
}


