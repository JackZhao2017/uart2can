/*	Created by Jack 2017/9/6 */
#include <stdio.h>
#include <stdlib.h>
#include "includes.h"
#include "usart2.h"
#include "task_config.h"
#include "task_uart_read.h"
#include "task_uart_parse.h"
#include "task_uart_send.h"


uint8_t  UART_TaskPause(void)
{
	 if(UARTRead_TaskPause()==0){
			goto err;
	 } 
	 if(UARTSend_TaskPause()==0){
		  goto err;
	 }
	 return 1;
err:
	 UARTRead_TaskPause();
	 UARTSend_TaskPause();
	 return 0;
}


uint8_t UART_TaskStart(void)
{
	uint8_t val=0;	  
	if(UARTSend_TaskStart()){
		val|=1;
	}
	if(UARTRead_TaskStart())
	{
		val|=2;
	}
	return val;
}
void UART_TaskInit(void)
{
	UARTParse_TaskInit();
	UARTRead_TaskInit();
	UARTSend_TaskInit();
}





