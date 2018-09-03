#ifndef _CAN_TASK_READ_H_
#define _CAN_TASK_READ_H_

#include "includes.h"



void    CAN_ReadTaskInit(void);
uint8_t CAN_ReadTaskStart(void);
uint8_t CAN_ReadTaskPause(void);
uint8_t CAN_GetReadTaskStatus(void);
void    CAN_ResolveMsg(CanRxMsg *msg);


#endif


