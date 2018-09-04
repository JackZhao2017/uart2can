#ifndef _TASKCONFIG_H_
#define _TASKCONFIG_H_

#include "includes.h"
/////////////////////////UCOSII��������///////////////////////////////////

//START ����

#define START_TASK_PRIO				 17 //��ʼ��������ȼ�����Ϊ���
#define START_STK_SIZE		 		  256  //���������ջ��С

#define SCHEDULE_INDICATE_TASK_PRIO   14 	//�����������ȼ�
#define SCHEDULE_INDICATE_STK_SIZE    64 	//���������ջ��С

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


#define READ_TASK_PRIO       		  11 	//�����������ȼ�
#define SEND_TASK_PRIO       		  9 	//�����������ȼ�
#define PARSE_TASK_PRIO               10

#define SEND_STK_SIZE  		    	  96 	//���������ջ��С
#define READ_STK_SIZE  		    	  64 	//���������ջ��С
#define PARSE_STK_SIZE  		      128	//���������ջ��С

#define TASK_ON   1
#define TASK_OFF  0


#endif
