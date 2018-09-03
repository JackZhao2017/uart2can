#ifndef _UART_TASK_H_
#define _UART_TASK_H_
#include "includes.h"

void UARTSend_TaskInit(void);


uint8_t UARTSend_TaskStart(void);
uint8_t UARTSend_TaskPause(void);

void 	UARTSend_OneFrame(void);
uint8_t UARTSend_GetTaskStatus(void);



#endif
