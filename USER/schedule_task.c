/*	Created by Jack 2017/9/6 */


#include "taskconfig.h"
#include "can_speed.h"
#include "led.h"
#include "delay.h"
#include "includes.h"
#include "can.h"
#include "schedule_task.h"
#include "can_task.h"


OS_STK    SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE];
OS_TMR    *tmr;
u8        tmrname[]="tmr";
volatile	s8 g_system_state=0;
volatile  u8 g_cur_mode=0;
volatile  u8 g_isvaild=VAILD;
void set_system_status(s8 status)
{
			g_system_state=status;
}

void set_canbus_curmode(u8 mode)
{
			g_cur_mode=mode;
}

void set_canbus_datamode(u8 mode)
{
			g_isvaild=mode;
}

void schedule_indicate_task(void *pdata)
{	 	
		u8 	err;
		u16  ledcount=0;
		u8  led_on=1;
		u8	scale=20;
		u16 period=100;
	  tmr=OSTmrCreate(5,5,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)speed_send_task,0,tmrname,&err);		//50msÖ´ÐÐÒ»´Î
		OSTmrStart(tmr,&err);	
		while(1)
		{		
				if(g_isvaild){
						g_speed&=0x00ff;
				}else {
						g_speed|=0xff00;
				}		
				
				if(g_system_state>0){
						scale=50;
						period=100;
				}else if(g_system_state<0){
						period=50;
						scale =period/2;
				}else if(g_system_state==0){
						scale=10;
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
				OSTimeDlyHMSM(0,0,0,10);
		};
}
