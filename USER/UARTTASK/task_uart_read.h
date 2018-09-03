#ifndef _UART_TASK_READ_H_
#define _UART_TASK_READ_H_

#include "includes.h"

uint8_t UARTRead_TaskStart(void);
uint8_t UARTRead_TaskPause(void);
void    UARTRead_TaskInit(void);
void 		UARTRead_StartReadOne(void);
#endif
