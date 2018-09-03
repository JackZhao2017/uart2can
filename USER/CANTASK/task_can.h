#ifndef _CAN_TASK_H_
#define _CAN_TASK_H_

#include "includes.h"
#include "can.h"
#include "task_can_read.h"
#include "task_can_send.h"

void    CAN_TaskInit(BAUDRATE brandrate);
uint8_t CAN_TaskStart(void);
uint8_t CAN_TaskPause(void);


void    CAN_SetBrandrate(BAUDRATE brandrate);
uint8_t CAN_GetBrandrate(void );
void    CAN_SetFilter(FILTER *filter,uint8_t len);

uint8_t CAN_GetReadMsg(CanRxMsg *msg);
uint8_t CAN_GetHeadReadMsg(CanRxMsg **msg);
void    CAN_DelHeadReadMsg(void);
void    CAN_SendPutMsg(u8 *msg);

#endif


