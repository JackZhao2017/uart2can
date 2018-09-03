#ifndef _UART_TASK_H_
#define _UART_TASK_H_
#include "includes.h"

#include "task_uart_read.h"
#include "task_uart_parse.h"
#include "task_uart_send.h"

void UART_TaskInit(void);

uint8_t UART_TaskStart(void);
uint8_t UART_TaskPause(void);


#endif
