#ifndef _TASKCONFIG_H_
#define _TASKCONFIG_H_

#include "includes.h"
/////////////////////////UCOSII任务设置///////////////////////////////////

//START 任务

#define START_TASK_PRIO				 17 //开始任务的优先级设置为最低
#define START_STK_SIZE		 		  256  //设置任务堆栈大小

#define SCHEDULE_INDICATE_TASK_PRIO   14 	//设置任务优先级
#define SCHEDULE_INDICATE_STK_SIZE    64 	//设置任务堆栈大小

#define CHK_STK_SIZE                  96
#define CHK_STK_PRIO                  15

#define CAN_READ_PRIO				   7 
#define CAN_READ_STK_SIZE			  64

#define CAN_SEND_PRIO				  8
#define CAN_SEND_STK_SIZE			  0

#define CAN_MUTEX_PRIO                3
#define UART_MUTEX_PRIO               5
#define CMD_MUTEX_PRIO                6
#define CANSEND_MUTEX_PRIO            4


#define READ_TASK_PRIO       		  11 	//设置任务优先级
#define SEND_TASK_PRIO       		  9 	//设置任务优先级
#define PARSE_TASK_PRIO               10

#define SEND_STK_SIZE  		    	  96 	//设置任务堆栈大小
#define READ_STK_SIZE  		    	  64 	//设置任务堆栈大小
#define PARSE_STK_SIZE  		      128	//设置任务堆栈大小

#define TASK_ON   1
#define TASK_OFF  0


#endif
