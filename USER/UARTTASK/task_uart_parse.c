/*	Created by Jack 2017/9/6 */
#include <stdio.h>
#include <stdlib.h>

#include "includes.h"

#include "task_config.h"
#include "uart_ringbuf.h"
#include "uart_detect.h"
#include "task_can.h"


OS_STK  UART_PARSE_TASK_STK[PARSE_STK_SIZE];

static 	 OS_EVENT * g_sem_parse;
static int uart_parse_status=0;
static u8 receivebuf[256]; 
static u8 parsebuf[128]={0x10 ,0x1 , 0x8,  0x0,  0x9,  0x6c,  0xfe,  0x0,  0xa7,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x66};
static RINGBUFFER g_ringInfo;


void  UARTParse_StartParseOne(u8 *buf,int len){
	if(!uart_parse_status){
		printf("%s \r\n",__func__);
		return ;
	}
	Ringbuffer_PutDataToBuffer(&g_ringInfo,buf,len);
	OSSemPost(g_sem_parse);
}


static void uart_parse_task(void *pdata)
{
	u8 err,type=0,syncindex;
	int getlen=0;
	uart_parse_status=1;
	while(1){

		getlen=0;
		OSSemPend(g_sem_parse,0,&err);
		getlen=Ringbuffer_GetbufferHeardata(&g_ringInfo,parsebuf,128);
		for(syncindex=0;syncindex<getlen;syncindex++)
		{
		 	if(!UART_DetectvaildSync(parsebuf,syncindex)){
		 		continue;
			}
			if(++syncindex>=getlen){
			 	syncindex-=1;
			 	break;
			}
			if(UART_DetectvaildMsgtype(&parsebuf[syncindex],&type)){
			 	if(syncindex+type-1>=getlen){
			 		syncindex-=1;
			 		break;
			 	}
			 	if(UART_Detectcrc8(&parsebuf[syncindex],0,type-1)){
					if(type==CANMSG){
						CAN_SendPutMsg(&parsebuf[syncindex]);
					}
					syncindex+=type-1;
			 	}
			}else{
					syncindex-=1;
			}
		}
		Ringbuffer_AddBufferHearaddr(&g_ringInfo,syncindex);
	}
}

void UARTParse_TaskInit(void)
{
	INT16U id_parse=PARSE_TASK_PRIO;
	g_sem_parse=OSSemCreate(0);
	Ringbuffer_Init(&g_ringInfo,receivebuf,sizeof(receivebuf));
	OSTaskCreateExt(uart_parse_task,
									(void *)0,
									(OS_STK *)&UART_PARSE_TASK_STK[PARSE_STK_SIZE-1],
									PARSE_TASK_PRIO,
									id_parse,
									(OS_STK *)&UART_PARSE_TASK_STK[0],
									 PARSE_STK_SIZE,
									(void *)0,
									OS_TASK_OPT_STK_CHK
				); 
	printf("%s \r\n",__func__);
}





