#ifndef _TASKCONFIG_H_
#define _TASKCONFIG_H_

#include "includes.h"
/////////////////////////UCOSII任务设置///////////////////////////////////

//START 任务

#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
#define START_STK_SIZE  					64  //设置任务堆栈大小


#define SEND_TASK_PRIO       			5 	//设置任务优先级
#define SEND_STK_SIZE  		    		64 	//设置任务堆栈大小


//LED任务
#define SCHEDULE_INDICATE_TASK_PRIO       			7 	//设置任务优先级
#define SCHEDULE_INDICATE_STK_SIZE  		    		64 	//设置任务堆栈大小

#define CAN_OBDII_PRIO						6  
#define CAN_OBDII_STK_SIZE				256

#define TASK_ON   1
#define TASK_OFF  0

extern OS_STK OBDII_TASK_STK[CAN_OBDII_STK_SIZE];
void obdii_task(void *pdata);
void resolve_obdii_speed(u8 *data);
u8 get_obdii_task_status(void);
s8 set_obdii_task_status(u8 status);


void speed_send_task(OS_TMR *ptmr, void *pdata);

void schedule_indicate_task(void *pdata);        	//任务函数
extern OS_STK SCHEDULE_INDICATE_TASK_STK[SCHEDULE_INDICATE_STK_SIZE];	//任务堆栈	
void set_system_status(s8 status);
#endif
