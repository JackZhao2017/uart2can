/*	Created by Jack 2017/9/6 */
#include <stdio.h>
#include <stdlib.h>

#include "includes.h"
#include "usart2.h"
#include "task_config.h"
#include "task_uart_parse.h"


OS_STK  UART_READ_TASK_STK[READ_STK_SIZE];
static volatile INT8U g_read_finished=1;
static u8 uart_read_status =0;
static volatile int finished=1;
static OS_EVENT *g_sem_read;

void UARTRead_StartReadOne(void)
{
	if(!finished)
		return ;
	OSSemPost(g_sem_read);
}


static void uart_read_task(void *pdata)
{
	u8 err;
	int len =0;
	uart_read_status=1;
	USART2_RX_STA=0;
	while(1){
		OSSemPend(g_sem_read,0,&err);
		finished=0;
		if(USART2_RX_STA&0X8000){				
			len=USART2_RX_STA&0X3FFF;							
			UARTParse_StartParseOne(USART2_RX_BUF,len);			
			USART2_RX_STA=0;							
		}
		finished=1;
	}
}

uint8_t UARTRead_TaskPause(void)
{
	INT8U err=0,count=0;
	if(!uart_read_status){
		return 1;
	}
	err = OSTaskSuspend(READ_TASK_PRIO);
	while(err){
		printf("%s err =%d \r\n",__func__,err);
		err = OSTaskSuspend(READ_TASK_PRIO);
		if(++count==5){
			return 0;
		}
	}
	uart_read_status=0;
	return 1;
}
uint8_t UARTRead_TaskStart(void)
{
	INT8U err=0,count=0;
	if(uart_read_status){
		 return 1;
	}
	err = OSTaskResume(READ_TASK_PRIO);
	while(err){
		printf("%s err =%d \r\n",__func__,err);
		err = OSTaskResume(READ_TASK_PRIO);
		if(++count==5){
			return 0;
		}
	}
	uart_read_status=1;
	return 1;
}

void UARTRead_TaskInit(void)
{
	INT16U id_read=READ_TASK_PRIO;
	g_sem_read=OSSemCreate(0);
	OSTaskCreateExt(uart_read_task,
					(void *)0,
					(OS_STK *)&UART_READ_TASK_STK[READ_STK_SIZE-1],
					READ_TASK_PRIO,
					id_read,
					(OS_STK *)&UART_READ_TASK_STK[0],
					READ_STK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK
	); 
	printf("%s \r\n",__func__);
}





