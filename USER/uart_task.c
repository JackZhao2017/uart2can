/*	Created by Jack 2017/9/6 */
#include <stdio.h>
#include <stdlib.h>

#include "includes.h"
#include "taskconfig.h"
#include "can_speed.h"
#include "uart_task.h"
#include "can_task.h"
#include "Encode.h"
#include "usart2.h"
#include "can_queue.h"
#include "ringbuf.h"
#include "detect.h"
#include "cmd_queue.h"
#include "control.h"
#include "crc8.h"



OS_STK  UART_SEND_TASK_STK[SEND_STK_SIZE];
OS_STK  UART_READ_TASK_STK[READ_STK_SIZE];
OS_STK  UART_PARSE_TASK_STK[READ_STK_SIZE];

static 	 OS_EVENT * g_sem_send;
static 	 OS_EVENT * g_mutex_read;

static volatile INT8U g_send_finished=1;
static volatile INT8U g_read_finished=1;

static u8 receivebuf[128]; 
static u8 parsebuf[32]={0x10 ,0x1 , 0x8,  0x0,  0x9,  0x6c,  0xfe,  0x0,  0xa7,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x66};
static RINGBUFFER ringInfo;


#define  lock     OSMutexPend(g_mutex_read,0,&err)
#define  unlock   OSMutexPost(g_mutex_read)

static void uart_read_task(void *pdata)
{
		INT8U err;
		int len =0;
		RINGBUFFER info;
		while(1){
			if(USART2_RX_STA&0X8000){		
				len=USART2_RX_STA&0X3FFF;							
				USART2_RX_STA=0;
				lock;
				memcpy(&info,&ringInfo,sizeof(RINGBUFFER));
				unlock;
				putdatatoBuffer(&info,USART2_RX_BUF,len);
				lock;
				flush(&ringInfo,&info);
				unlock;
			}else{
				OSTimeDlyHMSM(0,0,0,5);  
			}
		}
}
static void uart_parse_task(void *pdata)
{
	u8 i=0,err=0,type=0;
	RINGBUFFER info,info_t;
	while(1){
		if(ringInfo.num>9){
		
			lock;
			memcpy(&info,&ringInfo,sizeof(RINGBUFFER));
			unlock;

			if(detectvaildSync(&info,0x55)){
				if(detectvaildMsgtype(&info,&type)==1){
					info_t=info;
					if(getdatafromBuffer(&info_t,parsebuf,type+1)){
						if(detectcrc8(parsebuf,0,type)){
							if(type==CANMSG){
									putcansendPacket(parsebuf);
									startcansend();
							}
							else if(type==CMDMSG){
									putcontrolCmd(parsebuf);
							}
							info=info_t;
						}else{
							for(i=0;i<type+1;i++){
								printf("0x%x  ",parsebuf[i]);
							}
							printf("\r\n");
						}
					}
				}
			}

			lock;
			flush(&ringInfo,&info);
			unlock;

		}else{
			OSTimeDlyHMSM(0,0,0,5);
		}
	}
}

void uartsendstart(void)
{
	 if(!g_send_finished)
	 	return ;
	 g_send_finished=0;
	 OSSemPost(g_sem_send);
}
static void uart_send_task(void *pdata)
{
	    INT8U err;
		UartTxMsg usrtmsg;
		while(1)
		{
			OSSemPend(g_sem_send,0,&err);
			if(get_send_packet(&usrtmsg)){
				  UART_SEND_DATA((char *)&usrtmsg,usrtmsg.TYPE+2);
			}
			g_send_finished=1;
		}
}

void uart_task_Init(void)
{
	  INT8U err; 
	  g_sem_send=OSSemCreate(0);
	  g_mutex_read=OSMutexCreate(UART_MUTEX_PRIO,&err);
	  OSTaskCreate(uart_send_task,(void *)0,(OS_STK*)&UART_SEND_TASK_STK[SEND_STK_SIZE-1],SEND_TASK_PRIO);
	  ringbufferInit(&ringInfo,receivebuf,sizeof(receivebuf));
	  OSTaskCreate(uart_read_task,(void *)0,(OS_STK*)&UART_READ_TASK_STK[READ_STK_SIZE-1],READ_TASK_PRIO);
	  OSTaskCreate(uart_parse_task,(void *)0,(OS_STK*)&UART_PARSE_TASK_STK[READ_STK_SIZE-1],PARSE_TASK_PRIO);
}





