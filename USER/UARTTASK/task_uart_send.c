/*	Created by Jack 2017/9/6 */
#include <stdio.h>
#include <stdlib.h>

#include "includes.h"
#include "usart2.h"

#include "task_config.h"
#include "task_uart.h"
#include "task_can.h"
#include "uart_ringbuf.h"
#include "can_speed.h"
#include "uart_encode.h"
#include "uart_detect.h"
#include "crc8.h"



OS_STK  UART_SEND_TASK_STK[SEND_STK_SIZE];
static 	 OS_EVENT * g_sem_send;


static volatile INT8U g_send_finished=1;
static u8 uart_send_status =0;

void UARTSend_OneFrame(void)
{
	 if(!g_send_finished)
	 	return ;
	 g_send_finished=0;
	 OSSemPost(g_sem_send);
}


INT8U UARTSend_GetTaskStatus(void)
{
	return uart_send_status;
} 

static void uart_send_task(void *pdata)
{
	INT8U err;
	UartTxMsg usrtmsg;
	u16 speed=0;
	u8 valid=0;	 

	printf("uart_send_task inited 1\r\n");

	uart_send_status=1;
	while(1)
	{
		OSSemPend(g_sem_send,50,&err);

		if(UART_ENCODE_GetSend(&usrtmsg)){			      
			UART_SEND_DATA((char *)&usrtmsg,usrtmsg.TYPE+1);
		}else{
			speed=SPEED_GetCanSpeed();
			valid=SPEED_GetCanBusStatus();
			if(valid){
				speed&=0x00ff;
			}else{
				speed|=0xff00;
			}
			SPEED_UartSend(speed);	
		}
		g_send_finished=1;
	}
}


uint8_t UARTSend_TaskPause(void)
{
	INT8U err=0,count=0;
	if(!uart_send_status){
		return 1;
	}	

	err = OSTaskSuspend(SEND_TASK_PRIO);

	while(err){
		printf("%s err =%d \r\n",__func__,err);
		err = OSTaskSuspend(SEND_TASK_PRIO);
		if(++count==5){
			return 0;
		}
	}
	uart_send_status=0;
	return 1;
}	



uint8_t UARTSend_TaskStart(void)
{
	INT8U err=0,count=0;

	if(uart_send_status){
		 return 1;
	}

	err = OSTaskResume(SEND_TASK_PRIO);

	while(err){
		printf("%s err =%d \r\n",__func__,err);
		err = OSTaskResume(SEND_TASK_PRIO);
		if(++count==5){
				return 0;
		}
	}

	uart_send_status=1;
	return 1;
}



void UARTSend_TaskInit(void)
{
	INT16U id_send =SEND_TASK_PRIO;
	g_sem_send=OSSemCreate(0);
	OSTaskCreateExt(uart_send_task,
					(void *)0,
					(OS_STK *)&UART_SEND_TASK_STK[SEND_STK_SIZE-1],
					SEND_TASK_PRIO,
					id_send,
					(OS_STK *)&UART_SEND_TASK_STK[0],
					CAN_SEND_STK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK
					); 
	//OSTaskCreate(uart_send_task,(void *)0,(OS_STK*)&UART_SEND_TASK_STK[SEND_STK_SIZE-1],SEND_TASK_PRIO);
	printf("%s \r\n",__func__);
}





