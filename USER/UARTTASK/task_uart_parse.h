#ifndef _UART_TASK_PARSE_H_
#define _UART_TASK_PARSE_H_
#include "includes.h"

void  UARTParse_TaskInit(void);
void  UARTParse_StartParseOne(u8 *buf,int len);

#endif
