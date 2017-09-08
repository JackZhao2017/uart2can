/*	Created by Jack 2017/9/6 */


#include "taskconfig.h"
#include "can_speed.h"
#include "led.h"
#include "delay.h"
#include "includes.h"


OS_STK SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE];

OS_TMR *tmr;
u8      tmrname[]="tmr";
volatile	s8 g_can_protocol=0;

void set_system_status(s8 status)
{
			g_can_protocol=status;
}

void schedule_indicate_task(void *pdata)
{	 	
		u8 err;
		u16 scale=920;
		u16 period=1000;
	  tmr=OSTmrCreate(5,5,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)speed_send_task,0,tmrname,&err);		//50msÖ´ÐÐÒ»´Î
		OSTmrStart(tmr,&err);
		while(1)
		{
				if(g_can_protocol>0){
						scale=500;
						period=1000;
				}else if(g_can_protocol<0){
						period=500;
						scale =period/2;
				}else if(g_can_protocol==0){
						scale=80;
						period=1000;
				}
				LED0=0;
				LED1=1;
				LED_ON();
				delay_ms(period-scale);
				LED0=1;
				LED1=0;
				LED_OFF();
				delay_ms(scale);
		};
}
