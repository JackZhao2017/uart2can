#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include "includes.h"
/////////////////////////UCOSII任务设置///////////////////////////////////

//START 任务


#define SEND_TASK_PRIO       			5 	//设置任务优先级
#define SEND_STK_SIZE  		    		64 	//设置任务堆栈大小


//LED任务
#define SCHEDULE_INDICATE_TASK_PRIO       			9 	//设置任务优先级
#define SCHEDULE_INDICATE_STK_SIZE  		    		64 	//设置任务堆栈大小


void speed_send_task(OS_TMR *ptmr, void *pdata);

void schedule_indicate_task(void *pdata);        	//任务函数
extern OS_STK SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE];	//任务堆栈	
void set_system_status(s8 status);

void set_canbus_curmode(u8 mode);

void set_canbus_datamode(u8 mode);

#endif
