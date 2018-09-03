/*
	Created by Jack 2017/9/7
*/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "includes.h" 
#include "task_config.h"
#include "task_schedule.h"
#include "task_can_read.h"
#include "task_can.h"
#include "task_uart.h"
#include "frameware.h"
#include "crc8.h"
#include "usart2.h"
#include "can.h"

OS_STK START_TASK_STK[START_STK_SIZE];//ÈÎÎñ¶ÑÕ»	
void start_task(void *pdata);					//ÈÎÎñº¯Êý
void chk_task(void *pdata);
OS_STK CHK_TASK_STK[CHK_STK_SIZE];//ÈÎÎñ¶ÑÕ


BAUDRATE baudrate;

int main()
{
	INT16U id=START_TASK_PRIO;
	FRAMEWORK_Init();
	OSInit();

	OSTaskCreateExt(start_task,
					(void *)0,
					(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],
					START_TASK_PRIO,
					id,
					(OS_STK *)&START_TASK_STK[0],
					START_STK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK);  
	//OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//´´½¨ÆðÊ¼ÈÎÎñ
	OSStart();	
}

void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
	OSTimeDlyHMSM(0,0,2,5);
	OS_ENTER_CRITICAL();						//½øÈëÁÙ½çÇø(ÎÞ·¨±»ÖÐ¶Ï´ò¶Ï)   
	printf("start_task\r\n");
	crcInit(LSB,POLY);
	baudrate.CAN_SJW=CAN_SJW_1tq;
	baudrate.CAN_BS1=CAN_BS1_3tq;
	baudrate.CAN_BS2=CAN_BS2_2tq;
	baudrate.CAN_Prescaler=CAN_250kbps;
	CAN_TaskInit(baudrate);
	UART_TaskInit();
	SCHEDULE_TaskInit();	
	// OSTaskCreateExt(chk_task,
	// 					(void *)0,
	// 					(OS_STK *)&CHK_TASK_STK[CHK_STK_SIZE-1],
	// 					CHK_STK_PRIO,
	// 					CHK_STK_PRIO,
	// 					(OS_STK *)&CHK_TASK_STK[0],
	// 					CHK_STK_SIZE,
	// 					(void *)0,
	// 					OS_TASK_OPT_STK_CHK); 

	OSTaskSuspend(START_TASK_PRIO);	//¹ÒÆðÆðÊ¼ÈÎÎñ.
	OS_EXIT_CRITICAL();							//ÍË³öÁÙ½çÇø(¿ÉÒÔ±»ÖÐ¶Ï´ò¶Ï)
}
void chk_task(void *pdata)
{

	INT8U err;
	OS_STK_DATA stk_data;
	while(1)
	{
		err = OSTaskStkChk(SCHEDULE_INDICATE_TASK_PRIO,&stk_data);
		printf("SCHEDULE_INDICATE_TASK_PRIO err %d   used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));			


		OSTaskStkChk(START_TASK_PRIO,&stk_data);
		printf("START_TASK_PRIO err %d  used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));


		if(CAN_GetReadTaskStatus()){
			OSTaskStkChk(CAN_READ_PRIO,&stk_data);
			printf("CAN_READ_PRIO err %d  used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));

		}

		if(CAN_GetSendTaskStatus()){
			OSTaskStkChk(CAN_SEND_PRIO,&stk_data);
			printf("CAN_SEND_PRIO err %d  used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));

		}

		OSTaskStkChk(READ_TASK_PRIO,&stk_data);
		printf("READ_TASK_PRIO err %d  used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));


		OSTaskStkChk(SEND_TASK_PRIO,&stk_data);
		printf("SEND_TASK_PRIO err %d  used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));		


		OSTaskStkChk(PARSE_TASK_PRIO,&stk_data);
		printf("PARSE_TASK_PRIO err %d  used/free:%d/%d  usage:%%%d\r\n",err,stk_data.OSUsed,stk_data.OSFree,(stk_data.OSUsed*100)/(stk_data.OSUsed+stk_data.OSFree));	

		OSTimeDlyHMSM(0,0,5,0);
	}
}





